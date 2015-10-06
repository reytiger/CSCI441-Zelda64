#pragma once
#include "Utils.hpp"

#include <vector>

#include "WorldObjects/WorldObjectBase.hpp"
#include "WorldObjects/BezierCurve.hpp"

class Track : public WorldObject {
public:
    ////////////////////////////////////////////////////////////////////////////
    // constructors
    Track() : WorldObject() {
        m_TrackCPointsFile = "assets/world/bezier-halo.csv";
        glChk();
        m_halo.loadFile(m_TrackCPointsFile);
        // loadControlPoints(m_TrackCPointsFile);
        glChk();
    };

    // Point eval(double u, double v) const;

    virtual void draw() const;

private:
    int m_numberOfCurves;

    // void drawGround() const;

    // set up for control points
    string m_TrackCPointsFile;
    // bool loadControlPoints(string filename);
    // control points
    // void drawControlPoints() const;
    // control curves
    std::vector<BezierCurve> m_curvesCPoints;

    void drawTrackBlocks();

    double m_zMin;
    double m_zMax;

    BezierCurve m_halo;
};