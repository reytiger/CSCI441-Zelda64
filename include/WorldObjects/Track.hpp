#pragma once
#include "Utils.hpp"

#include <vector>

#include "WorldObjects/WorldObjectBase.hpp"
#include "WorldObjects/BezierCurve.hpp"
#include "WorldObjects/CallListObject.hpp"


class Track : public WorldObject {
public:
    ////////////////////////////////////////////////////////////////////////////
    // constructors
    Track() : WorldObject(){};

    // Point eval(double u, double v) const;
    void init();

    Vec eval(double t);
    double evalAngle(double t); // returns 0-360;

    virtual void draw() const;

private:
    int m_numberOfCurves = 0;

    // set up for control points
    string m_TrackCPointsFile;
    std::vector<BezierCurve> m_curvesCPoints;
    // bool loadControlPoints(string filename);
    // control points
    // void drawControlPoints() const;
    // control curves


    // TODO: update after calllist works
    void drawTrackBlocks() const;
    CallListObject m_trackCallBack;

    BezierCurve m_halo;
};