#pragma once
#include "Utils.hpp"

#include <vector>

#include "WorldObjects/WorldObjectBase.hpp"

#include "WorldObjects/Point.hpp"
#include "WorldObjects/BezierCurve.hpp"
#include "WorldObjects/FlagBanner.hpp"
using namespace std;

class WorldSurface : public WorldObject {
public:
    ////////////////////////////////////////////////////////////////////////////
    // constructors
    WorldSurface() = default;

    Vec eval(double x, double z) const;

    virtual void draw() const;
    virtual void update(double t, double dt);
    void setControlPoints(std::vector<Vec> v) { m_controlPoints = v; }
    void setZmaxmin();
    void setTreesCPoints(std::vector<Vec> v) { m_trees = v; }

private:
    int m_numberOfCurves = 4;

    void drawGround() const;
    void drawTree() const;

    // set up for control points
    string m_WorldSurfaceCPointsFile;
    std::vector<Vec> m_controlPoints;
    std::vector<Vec> m_trees;
    // control points
    void drawControlPoints() const;
    // control curves
    std::vector<BezierCurve> m_curvesCPoints;

    double m_zMin;
    double m_zMax;
};
