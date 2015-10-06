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
    FirnensAkkkin() : WorldObject() {
        m_count                = 0;
        m_wingAngle            = 0.0f;
        m_tPos                 = 0.0f;
        m_angle                = 0.0f;
        m_targetAngleAkkkinDir = 0.0f;
        ControlCage            = true;
        BezierCurve            = true;
        m_akkkinCPointsFile = "assets/world/AkkkinControlPoints.csv";
        glChk();
        loadControlPoints(m_akkkinCPointsFile);
        glChk();
    };

    void drawAkkkin();
    virtual void draw() const;

    void setWingRot(float wingAngle) { m_wingAngle = wingAngle; };
    void setAngle(float angle) { m_angle = angle; };
    void setControlCage(bool value) { this->ControlCage = value; };
    void setBezierCurve(bool value) { this->BezierCurve = value; };
    void setTPos(float tPos) { m_tPos = tPos; };

    void increaseTPos();

    ////////////////////////////////////////////////////////////////////////////
    // accessors
    float getTPos() { return m_tPos; }
    float getWingRot() { return m_wingAngle; }
    float getAngle() { return m_angle; }

    bool getControlCage() { return ControlCage; }
    bool getBezierCurve() { return BezierCurve; }

    ////////////////////////////////////////////////////////////////////////////
    // modifiers
    void updateAnimation();

private:
    float m_wingAngle, m_tPos, m_angle;
    float m_targetAngleAkkkinDir;
    int m_currentCurve, m_numberOfCurves;
    int m_count;

    // parts of Akkkin
    void drawBody() const;
    void drawWing() const;

    // set up for control points
    string m_akkkinCPointsFile;
    vector<Point> controlPoints;
    // Cage
    void drawCage() const;
    void drawControlPoints() const;
    void drawCageLines() const;
    // Curve
    void drawBezierCurve() const;
    void renderBezierCurve(Point p0, Point p1, Point p2, Point p3,
                           int resolution) const;
    bool loadControlPoints(string filename);
    bool ControlCage, BezierCurve;
    Point evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3,
                              float t) const;
};