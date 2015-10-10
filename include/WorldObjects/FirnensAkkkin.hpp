#pragma once
#include "Utils.hpp"

#include <vector>

#include "WorldObjects/WorldObjectBase.hpp"

#include "WorldObjects/Point.hpp"
using namespace std;

/*******************************  DRAW AKKKIN  ********************************/
class FirnensAkkkin : public WorldObject {
public:
    ////////////////////////////////////////////////////////////////////////////
    // constructors
    FirnensAkkkin() = default;

    void drawAkkkin();
    virtual void draw() const;

    void setWingRot(float wingAngle) { m_wingAngle = wingAngle; };
    void setAngle(float angle) { m_angle = angle; };
    void setControlCage(bool value) { this->m_controlCage = value; };
    void setBezierCurve(bool value) { this->m_bezierCurve = value; };
    void setTPos(float tPos) { m_tPos = tPos; };

    void increaseTPos(double dt);

    ////////////////////////////////////////////////////////////////////////////
    // accessors
    float getTPos() { return m_tPos; }
    float getWingRot() { return m_wingAngle; }
    float getAngle() { return m_angle; }

    bool getControlCage() { return m_controlCage; }
    bool getBezierCurve() { return m_bezierCurve; }

    ////////////////////////////////////////////////////////////////////////////
    // modifiers
    void updateAnimation(double t, double dt);
    bool loadControlPoints(string filename);

private:
    bool m_controlCage           = true;
    bool m_bezierCurve           = true;
    int m_currentCurve           = 0;
    int m_numberOfCurves         = 0;
    float m_wingAngle            = 0.0f;
    float m_tPos                 = 0.0f;
    float m_angle                = 0.0f;
    float m_targetAngleAkkkinDir = 0.0f;

    // set up for control points
    string m_akkkinCPointsFile;
    vector<Point> m_controlPoints;

    // parts of Akkkin
    void drawBody() const;
    void drawWing() const;

    // Cage
    void drawCage() const;
    void drawControlPoints() const;
    void drawCageLines() const;

    // Curve
    void drawBezierCurve() const;
    void renderBezierCurve(Point p0, Point p1, Point p2, Point p3,
                           int resolution) const;
    Point evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3,
                              float t) const;
};