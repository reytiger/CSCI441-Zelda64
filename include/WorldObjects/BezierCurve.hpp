#pragma once
#include "Utils.hpp"

#include "WorldObjects/WorldObjectBase.hpp"

#include "WorldObjects/Point.hpp"

#include <vector>

class BezierCurve : public WorldObject {
public:
    BezierCurve() : WorldObject() {}
    // BezierCurve(const std::vector<Point> &v) : WorldObject(), points(v) {}

    virtual void draw() const;

    Vec eval(double t) const;

    void loadFile(const std::string &filename);

    bool drawCage = true;
    bool drawPath = true;

private:
    std::vector<Vec> points;
    // Table to cache curve points.
    std::vector<Vec> ss;
    std::vector<double> ts;

    void reloadArclengthTables(int resolution);
    Vec evalCubic(Vec p0, Vec p1, Vec p2, Vec p3, double t) const;
};
