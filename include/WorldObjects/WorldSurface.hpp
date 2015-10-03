#pragma once
#include "Utils.hpp"

#include <vector>

#include "WorldObjects/WorldObjectBase.hpp"

#include "WorldObjects/Point.hpp"
#include "WorldObjects/BezierCurve.hpp"
using namespace std;

class WorldSurface : public WorldObject {
public:
    ////////////////////////////////////////////////////////////////////////////
    // constructors
    WorldSurface() : WorldObject() {
        // WorldSurfaceCPointsFile = "assets/world/WorldSurfaceCPoints.csv";
        m_WorldSurfaceCPointsFile = "assets/world/WorldSurfaceCPoints.csv";
        // info("%s", WorldSurfaceCPointsFile);
        glChk();
        loadControlPoints(m_WorldSurfaceCPointsFile);
        glChk();
    };

    virtual void draw() const;

private:
    int m_numberOfCurves;

    // set up for control points
    string m_WorldSurfaceCPointsFile;
    vector<Point> m_controlPoints;
    bool loadControlPoints(string filename);
    // control points
    void drawControlPoints();

    // control curves
    std::vector<BezierCurve> m_curves;
};