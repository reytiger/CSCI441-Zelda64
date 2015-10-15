#include "WorldObjects.hpp"
#include "Material.hpp"

#include <fstream>

using namespace std;

// A new class is needed for the Flag's due to there animation. Flags have a
// uniqie animation that should look like they are in the wind.

/*************************  DRAW FLAG BANNER  *********************************/
void FlagBanner::draw() const {
    pushMatrixAnd([&]() {
        glTranslated(m_pos.x, m_pos.y, m_pos.z);
        glScalef(0.25, 0.25, 0.25);
        drawPole();
        glTranslated(0, 4, 0);
        drawFlag();
    });
}

void FlagBanner::drawFlag() const {
    pushMatrixAnd([&]() {
        // TODO: culling? lighting? What should we do here
        // TODO: set a texture.
        // num of points to draw
        double stepSize   = 0.1;
        double heightFlag = 5;
        glDisable(GL_CULL_FACE);
        Material::CyanRubber.set();
        glBegin(GL_TRIANGLES);
        {
            for (double i = 0; i < 1 - stepSize; i += stepSize) {
                Vec drawing = m_WindsCurve.eval_t(i);
                Vec next    = m_WindsCurve.eval_t(i + stepSize);

                drawing.y = m_height - 2.0 * heightFlag;
                next.y    = m_height - 2.0 * heightFlag;

                auto normal = drawing.cross(next);

                // first triangle
                glNormal3d(normal.x, normal.y, normal.z);
                glVertex3d(drawing.x, drawing.y, drawing.z);

                glNormal3d(normal.x, normal.y, normal.z);
                glVertex3d(drawing.x, drawing.y + heightFlag, drawing.z);

                glNormal3d(normal.x, normal.y, normal.z);
                glVertex3d(next.x, next.y, next.z);
                // second trianlge

                glNormal3d(normal.x, normal.y, normal.z);
                glVertex3d(drawing.x, drawing.y + heightFlag, drawing.z);

                glNormal3d(normal.x, normal.y, normal.z);
                glVertex3d(next.x, next.y, next.z);

                glNormal3d(normal.x, normal.y, normal.z);
                glVertex3d(next.x, next.y + heightFlag, next.z);
            }
        };
        glEnd();
        glEnable(GL_CULL_FACE);
        // glEnable(GL_LIGHTING);
    });
}

void FlagBanner::drawPole() const {
    pushMatrixAnd([&]() {
        // base of pole
        Vec target = m_WindsCurve.getVec().at(0);
        glTranslated(target.x, 0, target.z);
        // turn so its faccing up
        glRotatef(-90, 1, 0, 0);
        Material::Bronze.set();
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        gluCylinder(quadratic, 1, 0.8, m_height, 20, 2);

        // top of pole
        // Both the top and pole need rotated, either move up with z than rotate
        // each or rotate both and move along the z.
        glTranslated(0.0, 0.0, m_height);
        Material::Turquoise.set();
        glutSolidCone(1, 2, 20, 2);
    });
}

void FlagBanner::updateAnimation(double t, double /*dt*/) {
    // TODO: create a more relistic movement
    std::vector<Vec> currentVec = m_WindsCurve.getVec();
    double speed                = 4;
    // point 1
    Vec newCPoint = currentVec.at(1);
    newCPoint.z = sin(currentVec.at(1).z + t * speed);
    currentVec.at(1) = newCPoint;
    // point 2
    newCPoint   = currentVec.at(2);
    newCPoint.z = -sin(currentVec.at(2).z + t * speed);
    currentVec.at(2) = newCPoint;
    // point 3
    newCPoint   = currentVec.at(3);
    newCPoint.z = -sin(currentVec.at(3).z + t * speed);
    currentVec.at(3) = newCPoint;
    m_WindsCurve.setVec(currentVec);
    m_WindsCurve.recomputeCurve(100);
}

/*************************  SET UP FOR CURVES  ********************************/

void FlagBanner::init() {
    m_FlagCPointsFile = "assets/world/flagBanner.csv";
    glChk();
    m_WindsCurve.loadFile(m_FlagCPointsFile);
    glChk();
}

Point FlagBanner::eval(double /*u*/, double /*v*/) {
    // firt, get the x pos.
    // std::vector<Point> tmp;
    // for (int i = 0; i < m_curvesCPoints.size(); ++i) {
    //     BezierCurve currentCurve = m_curvesCPoints.at(i);
    //     double _u = (u - currentCurve.getXmin())
    //                 / (currentCurve.getXmax() - currentCurve.getXmin());
    //     tmp.push_back(m_curvesCPoints.at(i).evalCubicPoint(_u));
    // }

    // // now get the z position.
    // BezierCurve uCurve = BezierCurve(tmp);
    // double _v = (v - uCurve.getZmin()) / (uCurve.getZmax() -
    // uCurve.getZmin());
    // return uCurve.evalCubicPoint(_v);
    Point tmp;
    return tmp;
}
