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
}

void WorldSurface::drawControlPoints() {
    // Draw our control points
    float scaleDown = 0.1;
    for (int i = 0; i < m_controlPoints.size(); ++i) {
        glPushMatrix();
        {
            glColor3f(10 / 255.0, 200 / 255.0, 10 / 255.0);
            glTranslatef(m_controlPoints.at(i).getX(),
                         m_controlPoints.at(i).getY(),
                         m_controlPoints.at(i).getZ());
            glScalef(scaleDown, scaleDown, scaleDown);
            glutSolidSphere(1, 10, 10);
        };
        glPopMatrix();
    }
}

/*************************  SET UP FOR CURVES  ********************************/
////////////////////////////////////////////////////////////////////////////////
//  Load our control points from file and store them in a global variable.
bool WorldSurface::loadControlPoints(string filename) {
    string str;
    int numOfPoints = 0;
    ifstream file(filename.c_str());

    info("%s", filename);
    assert(file);

    // get the number of points to build
    getline(file, str);
    numOfPoints      = atoi(str.c_str());
    m_numberOfCurves = numOfPoints / 4;

    std::cout << m_numberOfCurves << std::endl;

    // go through each line. The cotrol file ahs the structure xn, yn, zn
    for (int i = 0; i < numOfPoints; ++i) {
        getline(file, str, ',');
        float x = atoi(str.c_str());

        getline(file, str, ',');
        float y = atoi(str.c_str());

        getline(file, str, '\n');
        float z = atoi(str.c_str());

        Point createPoint(x, y, z);
        m_controlPoints.push_back(createPoint);
    }

    for (int i = 0; i < m_numberOfCurves; ++i) {
        std::vector<Point> v;
        v.push_back(m_controlPoints.at(i * 4));
        v.push_back(m_controlPoints.at(i * 4 + 1));
        v.push_back(m_controlPoints.at(i * 4 + 2));
        v.push_back(m_controlPoints.at(i * 4 + 3));
        BezierCurve *create = new BezierCurve(v);
        m_curvesCPoints.push_back(*create);
    }
    return true;
}

Point WorldSurface::eval(double u, double v) const {
    Point target;
    std::vector<Point> tmp;
    for (int i = 0; i < m_curvesCPoints.size(); ++i) {
        tmp.push_back(m_curvesCPoints.at(i).evalCubicPoint(u));
    }
    return target;
}
