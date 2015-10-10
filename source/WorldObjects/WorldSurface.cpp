#include <fstream>
#include "WorldObjects/WorldSurface.hpp"

using namespace std;

// BezierCurve bezierCurve;

/*************************  DRAW WORLD SURFACE  *******************************/
void WorldSurface::draw() const {
    pushMatrixAnd([&]() {
        drawControlPoints();
        for (int i = 0; i < m_curvesCPoints.size(); ++i) {
            m_curvesCPoints.at(i).drawCurve();
        }
    });
    pushMatrixAnd([&]() {
        Point test = eval(-1, -1.8);
        glPushMatrix();
        {
            glColor3d(200 / 255.0, 10 / 255.0, 10 / 255.0);
            glTranslated(test.getX(), test.getY(), test.getZ());
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
    for (int i = 0; i < m_controlPoints.size(); ++i) {
        glPushMatrix();
        {
            glColor3d(10 / 255.0, 200 / 255.0, 10 / 255.0);
            glTranslated(m_controlPoints.at(i).getX(),
                         m_controlPoints.at(i).getY(),
                         m_controlPoints.at(i).getZ());
            glScaled(scaleDown, scaleDown, scaleDown);
            glutSolidSphere(1, 10, 10);
        };
        glPopMatrix();
    }
}

void WorldSurface::drawGround() const {
    // TODO: it works right now, assums a square world. change so it's dynamic
    pushMatrixAnd([&]() {
        double dt      = 0.25;
        int resolution = 4;
        for (double x = m_curvesCPoints.at(0).getXmin();
             x <= m_curvesCPoints.at(0).getXmax();
             x += dt) {
            for (double z = m_zMin; z <= m_zMax; z += dt) {
                Point location = eval(x, z);
                glPushMatrix();
                {
                    glColor3d(200 / 255.0, 200 / 255.0, 200 / 255.0);
                    glTranslated(
                        location.getX(), location.getY(), location.getZ());
                    glScaled(0.06, 0.06, 0.06);
                    glutSolidSphere(1, 10, 10);
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

        Point createPoint(x, y, z);
        m_controlPoints.push_back(createPoint);
    }

    if (m_controlPoints.size() > 0) {
        m_zMin = m_controlPoints.at(0).getZ();
        m_zMax = m_controlPoints.at(0).getZ();
    }

    for (int i = 0; i < m_numberOfCurves; ++i) {
        std::vector<Point> v;
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

Point WorldSurface::eval(double u, double v) const {
    // firt, get the x pos.
    std::vector<Point> tmp;
    for (int i = 0; i < m_curvesCPoints.size(); ++i) {
        BezierCurve currentCurve = m_curvesCPoints.at(i);
        double _u = (u - currentCurve.getXmin())
                    / (currentCurve.getXmax() - currentCurve.getXmin());
        tmp.push_back(m_curvesCPoints.at(i).evalCubicPoint(_u));
    }

    // now get the z position.
    BezierCurve uCurve = BezierCurve(tmp);
    double _v = (v - uCurve.getZmin()) / (uCurve.getZmax() - uCurve.getZmin());
    return uCurve.evalCubicPoint(_v);
}
