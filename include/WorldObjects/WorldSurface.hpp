#pragma once
#include "Utils.hpp"

#include <vector>

#include "WorldObjects/WorldObjectBase.hpp"

#include "WorldObjects/Point.hpp"
using namespace std;

class WorldSurface : public WorldObject {
public:
    ////////////////////////////////////////////////////////////////////////////
    // constructors
    WorldSurface() : WorldObject() {
        // WorldSurfaceCPointsFile = "assets/world/WorldSurfaceCPoints.csv";
        WorldSurfaceCPointsFile = "assets/world/WorldSurfaceCPoints.csv";
        // info("%s", WorldSurfaceCPointsFile);
        glChk();
        loadControlPoints(WorldSurfaceCPointsFile);
        glChk();
    };

    virtual void draw() const;

private:
    int m_numberOfCurves;

    // set up for control points
    string WorldSurfaceCPointsFile;
    vector<Point> controlPoints;
    bool loadControlPoints(string filename);
    // control points
    void drawControlPoints();
};