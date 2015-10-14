#include <fstream>
#include "WorldObjects/WorldSurface.hpp"

using namespace std;

// BezierCurve bezierCurve;

/*************************  DRAW WORLD SURFACE  *******************************/
void WorldSurface::draw() const {
    pushMatrixAnd([&]() { drawControlPoints(); });
    pushMatrixAnd([&]() {
        // TODO: the first point on a curve seems to return the last point...
        Vec test = eval(3, -2);
        glPushMatrix();
        {
            glColor3d(200 / 255.0, 10 / 255.0, 10 / 255.0);
            glTranslated(test.x, test.y, test.z);
            glScaled(0.2, 0.2, 0.2);
            glutSolidSphere(1, 10, 10);
        };
        glPopMatrix();

        test = eval(2, -2);
        glPushMatrix();
        {
            glColor3d(200 / 255.0, 10 / 255.0, 10 / 255.0);
            glTranslated(test.x, test.y, test.z);
            glScaled(0.2, 0.2, 0.2);
            glutSolidSphere(1, 10, 10);
        };
        glPopMatrix();



    });
    pushMatrixAnd([&]() { drawGround(); });
}

void WorldSurface::drawControlPoints() const {
    // Draw our control points
    float scaleDown = 0.1f;
    for (size_t i = 0; i < m_controlPoints.size(); ++i) {
        glPushMatrix();
        {
            glColor3d(10 / 255.0, 200 / 255.0, 10 / 255.0);
            glTranslated(m_controlPoints.at(i).x + pos().x,
                         m_controlPoints.at(i).y + pos().y,
                         m_controlPoints.at(i).z + pos().z);
            glScaled(scaleDown, scaleDown, scaleDown);
            glutSolidSphere(1, 10, 10);
        };
        glPopMatrix();
    }
}

void WorldSurface::drawGround() const {
    // TODO: it works right now, assums a square world. change so it's dynamic
    // Each curve has a Xmax/min and Zmax/min. So, the dt will build points
    // baised on these values. We could have curves that have different values
    // so each curve should have different logic.
    pushMatrixAnd([&]() {
        double dt = 0.5;
        for (double x = m_curvesCPoints.at(0).getXmin();
             x <= m_curvesCPoints.at(0).getXmax() - 2 * dt;
             x += dt) {
            for (double z = m_zMin; z <= m_zMax - 2 * dt; z += dt) {
                Vec location     = eval(x, z);
                Vec nextLocation = eval(x + dt, z + dt);
                // glPushMatrix();
                // {
                //     glColor3d(200 / 255.0, 200 / 255.0, 200 / 255.0);
                //     glTranslated(location.x, location.y, location.z);
                //     glScaled(0.06, 0.06, 0.06);
                //     glutSolidSphere(1, 10, 10);
                // };
                glPushMatrix();
                {
                    glDisable(GL_CULL_FACE);
                    glBegin(GL_TRIANGLES);
                    {
                        // first triangle
                        glColor3d(100 / 255.0, 120 / 255.0, 120 / 255.0);
                        glVertex3f(location.x, location.y, location.z);
                        glColor3d(10 / 255.0, 200 / 255.0, 10 / 255.0);
                        glVertex3f(
                            nextLocation.x, eval(x + dt, z).y, location.z);
                        glColor3d(10 / 255.0, 10 / 255.0, 150 / 255.0);
                        glVertex3f(
                            nextLocation.x, nextLocation.y, nextLocation.z);

                        // second triangle
                        glColor3d(100 / 255.0, 120 / 255.0, 120 / 255.0);
                        glVertex3f(location.x, location.y, location.z);
                        glVertex3f(
                            nextLocation.x, nextLocation.y, nextLocation.z);
                        glVertex3f(
                            location.x, eval(x, z + dt).y, nextLocation.z);
                    };
                    glEnd();
                    glEnable(GL_CULL_FACE);
                };
                glPopMatrix();
            }
        }
    });
}

/*************************  SET UP FOR CURVES  ********************************/
////////////////////////////////////////////////////////////////////////////////
//  Load our control points from file and store them in a global variable.
bool WorldSurface::loadControlPoints(string filename) {
    string str;
    int numOfPoints = 0;
    ifstream file;

    glChk();
    file.open(filename.c_str());
    if (!file) {
        error("Unable to load '%s'.", filename);
        exit(1);
    }

    // get the number of points to build
    getline(file, str);
    numOfPoints      = atoi(str.c_str());
    m_numberOfCurves = numOfPoints / 4;

    // go through each line. The cotrol file ahs the structure xn, yn, zn
    for (int i = 0; i < numOfPoints; ++i) {
        getline(file, str, ',');
        float x = static_cast<float>(atoi(str.c_str()));

        getline(file, str, ',');
        float y = static_cast<float>(atoi(str.c_str()));

        getline(file, str, '\n');
        float z = static_cast<float>(atoi(str.c_str()));

        // Point createPoint(x, y, z);
        Vec createVec = Vec(x, y, z);
        m_controlPoints.push_back(createVec);
    }

    if (m_controlPoints.size() > 0) {
        m_zMin = m_controlPoints.at(0).z;
        m_zMax = m_controlPoints.at(0).z;
    }

    for (int i = 0; i < m_numberOfCurves; ++i) {
        std::vector<Vec> v;
        v.push_back(m_controlPoints.at(i * 4));
        v.push_back(m_controlPoints.at(i * 4 + 1));
        v.push_back(m_controlPoints.at(i * 4 + 2));
        v.push_back(m_controlPoints.at(i * 4 + 3));
        BezierCurve create = BezierCurve(v);
        create.evalMaxMin();
        m_curvesCPoints.push_back(create);
        if (create.getZmin() < m_zMin) {
            m_zMin = create.getZmin();
        }
        if (create.getZmax() > m_zMax) {
            m_zMax = create.getZmax();
        }
    }
    return true;
}

Vec WorldSurface::eval(double x, double z) const {
    // firt, get the x pos.
    std::vector<Vec> tmp;
    for (size_t i = 0; i < m_curvesCPoints.size(); ++i) {
        BezierCurve currentCurve = m_curvesCPoints.at(i);
        double _x = (x - currentCurve.getXmin())
                    / (currentCurve.getXmax() - currentCurve.getXmin());

        tmp.push_back(m_curvesCPoints.at(i).eval_t(_x));
        if (i == 0) {
            // Vec tmp = eval_t(_x);
            // info("%s", m_curvesCPoints.at(i).eval_t(_x));
            // info("%s  %s", x, _x);
            // tmp.push_back(m_curvesCPoints.at(i).eval_t(0));

        } else {
        }
    }

    // now get the z position.
    BezierCurve uCurve = BezierCurve(tmp);
    double _z          = (z - uCurve.getZmin()) / (uCurve.getZmax() - uCurve.getZmin());

    return uCurve.eval_t(_z) + pos();
}
