#pragma once
#include "Utils.hpp"

#include "WorldObjects/WorldObjectBase.hpp"

#include "WorldObjects/Point.hpp"

#include <vector>

class BezierCurve : public WorldObject {
public:
    BezierCurve() : WorldObject() {}
    BezierCurve(const std::vector<Vec> &v) : WorldObject(), m_points(v) {
        evalMaxMin();
    }

    virtual void draw() const;
    virtual void drawCurve() const;

    Vec eval(double t) const;

    void loadFile(const std::string &filename);
    void evalMaxMin();

    bool drawCage = false;
    bool drawPath = false;

    double getXmin() const { return m_xMin; }
    double getXmax() const { return m_xMax; }
    double getZmin() const { return m_zMin; }
    double getZmax() const { return m_zMax; }

    void setVec(std::vector<Vec> v) { m_points = v; }
    std::vector<Vec> getVec() const { return m_points; }

    void recomputeCurve(int resolution) const;

private:
    std::vector<Vec> m_points;
    // Table to cache curve points.
    mutable std::vector<Vec> ss;
    mutable std::vector<double> ts;

    Vec evalCubic(Vec p0, Vec p1, Vec p2, Vec p3, double t) const;

    double m_xMin;
    double m_xMax;
    double m_zMin;
    double m_zMax;
};
