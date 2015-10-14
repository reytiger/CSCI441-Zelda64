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
    WorldSurface() = default;

    Vec eval(double x, double z) const;

    virtual void draw() const;
    bool loadControlPoints(string filename);

private:
    int m_numberOfCurves;

    void drawGround() const;
    void drawTree() const;

    // set up for control points
    string m_WorldSurfaceCPointsFile;
    vector<Vec> m_controlPoints;
    std::vector<Vec> m_trees;
    // Vec m_controlPoints;
    // control points
    void drawControlPoints() const;
    // control curves
    std::vector<BezierCurve> m_curvesCPoints;

    double m_zMin;
    double m_zMax;
};
