#include <fstream>
#include "WorldObjects/FirnensAkkkin.hpp"

#include "WorldObjects/Point.hpp"
using namespace std;

void FirnensAkkkin::increaseTPos(double dt) {
    m_tPos += dt * 0.5;
    if (m_tPos > 1) {
        m_currentCurve++;
        if (m_currentCurve >= m_numberOfCurves) {
            m_currentCurve = 0;
        }
        m_tPos = 0;
    }
}

void FirnensAkkkin::updateAnimation(double t, double dt) {
    m_wingAngle = sin(t * 6);
    increaseTPos(dt);

    // akkkin randomly tilts and slowly moves to angle for a few seconds
    setWingRot(sin(t * 6));
    if (fmod(t, 5.0) <= 0.1) {
        m_targetAngleAkkkinDir = getRandf() * 60.0f;
        if (getRandf() > 0.5f)
            m_targetAngleAkkkinDir *= -1;
    }
    double stepSize = 0.25;
    if (m_targetAngleAkkkinDir < getAngle()) {
        stepSize *= -1;
    } else if (m_targetAngleAkkkinDir == getAngle()) {
        stepSize = 0;
    }
    setAngle(getAngle() + stepSize);
}

/*******************************  DRAW AKKKIN  ********************************/
void FirnensAkkkin::draw() const {
    // Nothing to print.
    if (m_controlPoints.empty()) {
        return;
    }
    glPushMatrix();
    {
        drawCage();
        drawBezierCurve();

        glPushMatrix();
        {
            // check which curver we are on and where on that curve
            Point p0 = m_controlPoints.at(m_currentCurve * 3);
            Point p1 = m_controlPoints.at((m_currentCurve * 3) + 1);
            Point p2 = m_controlPoints.at((m_currentCurve * 3) + 2);
            Point p3 = m_controlPoints.at((m_currentCurve * 3) + 3);
            Point nextPos = evaluateBezierCurve(p0, p1, p2, p3, m_tPos);
            glTranslated(nextPos.getX(), nextPos.getY(), nextPos.getZ());

            glRotated(m_angle, 1, 0, 0);

            drawBody();
        };
        glPopMatrix();
    };
    glPopMatrix();
}

void FirnensAkkkin::drawBody() const {
    pushMatrixAnd([=]() {
        double scaleDown = 0.2f;
        glColor3d(200 / 255.0, 10 / 255.0, 10 / 255.0);

        glScaled(scaleDown, scaleDown, scaleDown);
        glutSolidSphere(1, 20, 20);

        // right wing
        glPushMatrix();
        {
            glTranslated(0.5, 0, 0.5);
            glRotated(90, 1, 0, 0);
            glRotated(m_wingAngle * 30, 1, 0, 0);
            drawWing();
        };
        glPopMatrix();
        // left wing
        glPushMatrix();
        {
            glScaled(1, -1, 1);
            glTranslated(0.5, 0, 0.5);
            glRotated(90, 1, 0, 0);
            glRotated(m_wingAngle * 30, 1, 0, 0);
            drawWing();
        };
        glPopMatrix();
    });
}

void FirnensAkkkin::drawWing() const {
    glPushMatrix();
    {
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();

        glColor3d(20 / 255.0, 20 / 255.0, 20 / 255.0);
        glRotated(-30, 1, 0, 0);
        gluCylinder(quadratic, 0.1, 0.1, 2, 10, 2); // bot

        glRotated(15, 1, 0, 0);
        glTranslated(0, 0.5, 1.75);
        gluCylinder(quadratic, 0.1, 0.1, 2, 10, 2); // middle
        // Draw fingers
        glPushMatrix();
        {
            glColor3d(196 / 255.0, 196 / 255.0, 196 / 255.0);
            glRotated(-45, 0, 1, 0);
            glRotated(20, 1, 0, 0);
            glScaled(1, 0.4, 1);
            gluCylinder(quadratic, 0.3, 0.1, 1.5, 10, 2); // fether
            glRotated(-50, 0, 1, 0);
            gluCylinder(quadratic, 0.3, 0.1, 2, 10, 2); // fether
        };
        glPopMatrix();

        glRotated(15, 1, 0, 0);
        glTranslated(0, 0.5, 1.75);
        // Draw fingers
        glPushMatrix();
        {
            glColor3d(196 / 255.0, 196 / 255.0, 196 / 255.0);
            glRotated(-45, 0, 1, 0);
            glRotated(20, 1, 0, 0);
            glScaled(1, 0.4, 1);
            gluCylinder(quadratic, 0.1, 0.1, 1, 10, 2); // fether
            glRotated(-50, 0, 1, 0);
            gluCylinder(quadratic, 0.1, 0.1, 1.5, 10, 2); // fether
        };
        glPopMatrix();
        glColor3d(20 / 255.0, 20 / 255.0, 20 / 255.0);
        glRotated(10, 0, 1, 0);
        gluCylinder(quadratic, 0.1, 0.1, 1, 10, 2); // top
    };
    glPopMatrix();
}

/*************************  SET UP FOR CURVES  ********************************/
////////////////////////////////////////////////////////////////////////////////
//  Load our control points from file and store them in a global variable.
bool FirnensAkkkin::loadControlPoints(string filename) {
    // TODO: load all the points from a yaml file
    return true;
}

// DRAW CAGE
// uses, drawControlPoints and drawCageLines
void FirnensAkkkin::drawCage() const {
    glPushMatrix();
    {
        if (m_controlCage) {
            drawControlPoints();
            drawCageLines();
        }
    };
    glPopMatrix();
}

void FirnensAkkkin::drawControlPoints() const {
    // Draw our control points
    double scaleDown = 0.1f;
    for (int i = 0; i < (signed)m_controlPoints.size(); ++i) {
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

void FirnensAkkkin::drawCageLines() const {
    // Draw yellow lines to connect the points
    for (int i = 0; i < (signed)m_controlPoints.size() - 1; ++i) {
        glPushMatrix();
        {
            glColor3d(245 / 255.0, 184 / 255.0, 0 / 255.0);
            glLineWidth(3.0f);
            glDisable(GL_LIGHTING);
            glBegin(GL_LINES);
            {
                glVertex3d(m_controlPoints.at(i).getX(),
                           m_controlPoints.at(i).getY(),
                           m_controlPoints.at(i).getZ());

                glVertex3d(m_controlPoints.at(i + 1).getX(),
                           m_controlPoints.at(i + 1).getY(),
                           m_controlPoints.at(i + 1).getZ());
            };
            glEnd();
            glEnable(GL_LIGHTING);
        };
        glPopMatrix();
    }
}

// DRAW THE CURVE!
void FirnensAkkkin::drawBezierCurve() const {
    if (m_bezierCurve) {
        // Draw the Bezier Curve!
        int resolution = 20;
        for (int i = 0; i < (signed)m_controlPoints.size() - 1; i += 3) {
            // check the 1, 2, 3, and 4th point starting at the current.
            renderBezierCurve(m_controlPoints.at(i),
                              m_controlPoints.at(i + 1),
                              m_controlPoints.at(i + 2),
                              m_controlPoints.at(i + 3),
                              resolution);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Responsible for drawing a Bezier Curve as defined by four control points.
//  Breaks the curve into n segments as specified by the resolution.
void FirnensAkkkin::renderBezierCurve(Point p0, Point p1, Point p2, Point p3,
                                      int resolution) const {
    glPushMatrix();
    {
        glColor3d(51 / 255.0, 102 / 255.0, 201 / 255.0);
        glDisable(GL_LIGHTING);
        glLineWidth(3.5f);
        glBegin(GL_LINES);
        {
            double t      = 0;  // should go from 0 - 1
            Point current = p0; // get the start location

            // draw the individ lines
            for (int i = 0; i < resolution; ++i) {
                Point next = evaluateBezierCurve(p0, p1, p2, p3, t);

                // draw stuff!!!!!
                glVertex3d(current.getX(), current.getY(), current.getZ());
                glVertex3d(next.getX(), next.getY(), next.getZ());

                current = next;
                // increment the step size, the (-1) gets us to step from 0 - 1
                t += 1 / ((double)resolution - 1);
            }
        };
        glEnd();
        glEnable(GL_LIGHTING);
    };
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////
// Computes a location along a Bezier Curve.
Point FirnensAkkkin::evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3,
                                         double t) const {
    // equation used from:
    //   https://en.wikipedia.org/wiki/B%C3%A9zier_curve
    // navigate to the Cubic Bézier curves
    Point target = (((double)pow((1 - t), 3) * p0)
                    + (3 * (double)pow((1 - t), 2) * t * p1)
                    + (3 * (double)(1 - t) * pow(t, 2) * p2)
                    + (pow(t, 3) * p3));

    return target;
}
