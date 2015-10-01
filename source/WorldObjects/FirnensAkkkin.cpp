#include <fstream>
#include "WorldObjects/FirnensAkkkin.hpp"

#include "WorldObjects/Point.hpp"
using namespace std;

void FirnensAkkkin::increaseTPos() {
  m_tPos += 0.015;
  if (m_tPos > 1) {
    currentCurve++;
    if (currentCurve >= numberOfCurves) {
      currentCurve = 0;
    }
    m_tPos = 0;
  }
}

/********************************************  DRAW AKKKIN
 * *************************************/
void FirnensAkkkin::draw() const {
  glPushMatrix();
  {
    drawCage();
    drawBezierCurve();

    glPushMatrix();
    {
      // check which curver we are on and where on that curve
      Point p0 = controlPoints.at(currentCurve * 3);
      Point p1 = controlPoints.at((currentCurve * 3) + 1);
      Point p2 = controlPoints.at((currentCurve * 3) + 2);
      Point p3 = controlPoints.at((currentCurve * 3) + 3);
      Point nextPos = evaluateBezierCurve(p0, p1, p2, p3, m_tPos);
      glTranslatef(nextPos.getX(), nextPos.getY(), nextPos.getZ());

      glRotatef(m_angle, 1, 0, 0);

      drawBody();
    };
    glPopMatrix();
  };
  glPopMatrix();
}

void FirnensAkkkin::drawBody() const {
  glPushMatrix();
  {
    float scaleDown = 0.2;
    glColor3f(200 / 255.0, 10 / 255.0, 10 / 255.0);

    glScalef(scaleDown, scaleDown, scaleDown);
    glutSolidSphere(1, 20, 20);

    // right wing
    glPushMatrix();
    {
      glTranslatef(0.5, 0, 0.5);
      glRotatef(90, 1, 0, 0);
      glRotatef(m_wingAngle * 30, 1, 0, 0);
      drawWing();
    };
    glPopMatrix();
    // left wing
    glPushMatrix();
    {
      glScalef(1, -1, 1);
      glTranslatef(0.5, 0, 0.5);
      glRotatef(90, 1, 0, 0);
      glRotatef(m_wingAngle * 30, 1, 0, 0);
      drawWing();
    };
    glPopMatrix();
  };
  glPopMatrix();
}

void FirnensAkkkin::drawWing() const {
  glPushMatrix();
  {
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glColor3f(20 / 255.0, 20 / 255.0, 20 / 255.0);
    glRotatef(-30, 1, 0, 0);
    gluCylinder(quadratic, 0.1, 0.1, 2, 10, 2); // bot

    glRotatef(15, 1, 0, 0);
    glTranslatef(0, 0.5, 1.75);
    gluCylinder(quadratic, 0.1, 0.1, 2, 10, 2); // middle
    // Draw fingers
    glPushMatrix();
    {
      glColor3f(196 / 255.0, 196 / 255.0, 196 / 255.0);
      glRotatef(-45, 0, 1, 0);
      glRotatef(20, 1, 0, 0);
      glScalef(1, 0.4, 1);
      gluCylinder(quadratic, 0.3, 0.1, 1.5, 10, 2); // fether
      glRotatef(-50, 0, 1, 0);
      gluCylinder(quadratic, 0.3, 0.1, 2, 10, 2); // fether
    };
    glPopMatrix();

    glRotatef(15, 1, 0, 0);
    glTranslatef(0, 0.5, 1.75);
    // Draw fingers
    glPushMatrix();
    {
      glColor3f(196 / 255.0, 196 / 255.0, 196 / 255.0);
      glRotatef(-45, 0, 1, 0);
      glRotatef(20, 1, 0, 0);
      glScalef(1, 0.4, 1);
      gluCylinder(quadratic, 0.1, 0.1, 1, 10, 2); // fether
      glRotatef(-50, 0, 1, 0);
      gluCylinder(quadratic, 0.1, 0.1, 1.5, 10, 2); // fether
    };
    glPopMatrix();
    glColor3f(20 / 255.0, 20 / 255.0, 20 / 255.0);
    glRotatef(10, 0, 1, 0);
    gluCylinder(quadratic, 0.1, 0.1, 1, 10, 2); // top
  };
  glPopMatrix();
}

/*******************************************  SET UP FOR CURVES
 * ********************************/

////////////////////////////////////////////////////////////////////////////////
//  Load our control points from file and store them in a global variable.
bool FirnensAkkkin::loadControlPoints(string filename) const {
  // TODO #04: read in control points from file.  Make sure the file can be
  // opened and handle it appropriately.
  string str;
  int numOfPoints = 0;
  ifstream file(filename.c_str());

  // get the number of points to build
  getline(file, str);
  numOfPoints = atoi(str.c_str());
  this->numberOfCurves = numOfPoints / 3;

  // go through each line. The cotrol file ahs the structure xn, yn, zn
  for (int i = 0; i < numOfPoints; ++i) {
    getline(file, str, ',');
    float x = atoi(str.c_str());

    getline(file, str, ',');
    float y = atoi(str.c_str());

    getline(file, str, '\n');
    float z = atoi(str.c_str());

    Point createPoint(x, y, z);
    controlPoints.push_back(createPoint);
  }
  return true;
}

// DRAW CAGE
// uses, drawControlPoints and drawCageLines
void FirnensAkkkin::drawCage() const {
  glPushMatrix();
  {
    if (ControlCage) {
      drawControlPoints();
      drawCageLines();
    }
  };
  glPopMatrix();
}

void FirnensAkkkin::drawControlPoints() const {
  // Draw our control points
  float scaleDown = 0.1;
  for (int i = 0; i < controlPoints.size(); ++i) {
    glPushMatrix();
    {
      glColor3f(10 / 255.0, 200 / 255.0, 10 / 255.0);
      glTranslatef(controlPoints.at(i).getX(), controlPoints.at(i).getY(),
                   controlPoints.at(i).getZ());
      glScalef(scaleDown, scaleDown, scaleDown);
      glutSolidSphere(1, 10, 10);
    };
    glPopMatrix();
  }
}

void FirnensAkkkin::drawCageLines() const {
  // Draw yellow lines to connect the points
  for (int i = 0; i < controlPoints.size() - 1; ++i) {
    glPushMatrix();
    {
      glColor3f(245 / 255.0, 184 / 255.0, 0 / 255.0);
      glLineWidth(3.0f);
      glDisable(GL_LIGHTING);
      glBegin(GL_LINES);
      {
        glVertex3f(controlPoints.at(i).getX(), controlPoints.at(i).getY(),
                   controlPoints.at(i).getZ());

        glVertex3f(controlPoints.at(i + 1).getX(),
                   controlPoints.at(i + 1).getY(),
                   controlPoints.at(i + 1).getZ());
      };
      glEnd();
      glEnable(GL_LIGHTING);
    };
    glPopMatrix();
  }
}

// DRAW THE CURVE!
void FirnensAkkkin::drawBezierCurve() const {
  if (BezierCurve) {
    // Draw the Bezier Curve!
    int resolution = 20;
    for (int i = 0; i < controlPoints.size() - 1; i += 3) {
      // check the 1, 2, 3, and 4th point starting at the current.
      renderBezierCurve(controlPoints.at(i), controlPoints.at(i + 1),
                        controlPoints.at(i + 2), controlPoints.at(i + 3),
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
    glColor3f(51 / 255.0, 102 / 255.0, 201 / 255.0);
    glDisable(GL_LIGHTING);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
    {
      float t = 0;        // should go from 0 - 1
      Point current = p0; // get the start location

      // draw the individ lines
      for (int i = 0; i < resolution; ++i) {
        Point next = evaluateBezierCurve(p0, p1, p2, p3, t);

        // draw stuff!!!!!
        glVertex3f(current.getX(), current.getY(), current.getZ());
        glVertex3f(next.getX(), next.getY(), next.getZ());

        current = next;
        // increment the step size, the (-1) gets us to step from 0 - 1
        t += 1 / ((float)resolution - 1);
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
                                         float t) const {
  // equation used from:
  //   https://en.wikipedia.org/wiki/B%C3%A9zier_curve
  // navigate to the Cubic Bézier curves
  Point target =
      (((float)pow((1 - t), 3) * p0) + (3 * (float)pow((1 - t), 2) * t * p1) +
       (3 * (float)(1 - t) * pow(t, 2) * p2) + (pow(t, 3) * p3));

  return target;
}