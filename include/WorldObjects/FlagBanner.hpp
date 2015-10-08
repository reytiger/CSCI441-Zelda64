#pragma once
#include "Utils.hpp"

#include <vector>

#include "WorldObjects/WorldObjectBase.hpp"

#include "WorldObjects/Point.hpp"
#include "WorldObjects/BezierCurve.hpp"
using namespace std;

class FlagBanner : public WorldObject {
public:
    ////////////////////////////////////////////////////////////////////////////
    // constructors
    FlagBanner() : WorldObject(){};

    void init();
    void updateAnimation(double t, double dt);

    Point eval(double u, double v);

    virtual void draw() const;

private:
    void drawFlag() const;
    void drawPole() const;

    BezierCurve m_WindsCurve;

    // set up for control points
    string m_FlagCPointsFile;

    vector<Point> m_controlPoints;
    bool loadControlPoints(string filename);
    void drawControlPoints() const;
    // control curves
};
