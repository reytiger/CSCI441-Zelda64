#include <fstream>
#include "WorldObjects/WorldSurface.hpp"

using namespace std;

void WorldSurface::update(double t, double dt) { WorldObject::update(t, dt); }

/*************************  DRAW WORLD SURFACE  *******************************/
void WorldSurface::draw() const {
    // drawControlPoints();
    pushMatrixAnd([&]() {
        // TODO: the first point on a curve seems to return the last point...
        Vec test = eval(3, -2) + pos();
        glPushMatrix();
        {
            glColor3d(200 / 255.0, 10 / 255.0, 10 / 255.0);
            glTranslated(test.x, test.y, test.z);
            glScaled(0.2, 0.2, 0.2);
            glutSolidSphere(1, 10, 10);
        };
        glPopMatrix();

        test = eval(2, -2) + pos();
        glPushMatrix();
        {
            glColor3d(200 / 255.0, 10 / 255.0, 10 / 255.0);
            glTranslated(test.x, test.y, test.z);
            glScaled(0.2, 0.2, 0.2);
            glutSolidSphere(1, 10, 10);
        };
        glPopMatrix();

        // draw the trees
        for (size_t i = 0; i < m_trees.size(); ++i) {
            glPushMatrix();
            {
                Vec target = eval(m_trees.at(i).x, m_trees.at(i).z);
                // glTranslated(m_trees.at(i).x, target.y, m_trees.at(i).z);
                glTranslated(target.x, target.y, target.z);
                drawTree();
            };
            glPopMatrix();
        }
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
        static Color color = randColor();
        color.glSet();
        double dt = 1.0;
        for (double x = m_curvesCPoints.at(0).getXmin();
             x <= m_curvesCPoints.at(0).getXmax() - 2 * dt;
             x += dt) {
            for (double z = m_zMin; z <= m_zMax - 2 * dt; z += dt) {
                Vec location     = eval(x, z);
                Vec nextLocation = eval(x + dt, z + dt);
                glPushMatrix();
                {
                    glDisable(GL_CULL_FACE);
                    glBegin(GL_TRIANGLES);
                    {
                        // TODO: Make this more reasonable.
                        Vec normal = Vec(0.0, 1.0, 0.0);

                        // first triangle
                        glNormal3d(normal.x, normal.y, normal.z);
                        glVertex3f(location.x, location.y, location.z);

                        glNormal3d(normal.x, normal.y, normal.z);
                        glVertex3f(
                            nextLocation.x, eval(x + dt, z).y, location.z);

                        glNormal3d(normal.x, normal.y, normal.z);
                        glVertex3f(
                            nextLocation.x, nextLocation.y, nextLocation.z);

                        // second triangle
                        glNormal3d(normal.x, normal.y, normal.z);
                        glVertex3f(location.x, location.y, location.z);

                        glNormal3d(normal.x, normal.y, normal.z);
                        glVertex3f(
                            nextLocation.x, nextLocation.y, nextLocation.z);

                        glNormal3d(normal.x, normal.y, normal.z);
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

void WorldSurface::drawTree() const {
    glPushMatrix();
    {
        glDisable(GL_CULL_FACE);

        // glTranslatef(objX, 0, objZ);
        // // draw the base of a tree
        // glRotatef(-90, 1, 0, 0); // draw the tree up from our current grid

        // glTranslatef(0, height / 2, 0);
        // glScalef(1, 1, height);
        // Rototae up!
        glRotatef(-90.0, 1.0, 0.0, 0.0);

        // base of tree, nice dark brown for the base
        glColor3f(134 / 255.0, 89 / 255.0, 45 / 255.0);
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, 0.2, 0.1, 0.5, 20, 2);
        // top of tree, nice green for the tops
        glTranslatef(0, 0, 0.5);
        glColor3f(100 / 255.0, 200 / 255.0, 100 / 255.0);
        glutSolidCone(0.15, 0.4, 20, 2);
        glEnable(GL_CULL_FACE);
    };
    glPopMatrix();
}

/*************************  SET UP FOR CURVES  ********************************/
////////////////////////////////////////////////////////////////////////////////
//  Load our control points from file and store them in a global variable.
bool WorldSurface::loadControlPoints(string filename) {
    string str;
    int numOfPoints = 0;
    ifstream file;

    // get the file needed
    glChk();
    file.open(filename.c_str());
    if (!file) {
        error("Unable to load '%s'.", filename);
        exit(1);
    }

    // load in the Control Points
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

    // Load in the Trees CPoints
    getline(file, str);
    int numOfTrees = atoi(str.c_str());
    for (int i = 0; i < numOfTrees; ++i) {
        getline(file, str, ',');
        float x = static_cast<float>(atoi(str.c_str()));

        getline(file, str, '\n');
        float z = static_cast<float>(atoi(str.c_str()));

        Vec createVec = Vec(x, 0.0, z);
        m_trees.push_back(createVec);
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
    double _z = (z - uCurve.getZmin()) / (uCurve.getZmax() - uCurve.getZmin());
    return uCurve.eval_t(_z) + pos();
}
