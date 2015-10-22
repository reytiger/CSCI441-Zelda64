#pragma once
#include "Utils.hpp"

#include "WorldObjects/WorldObjectBase.hpp"

#include <vector>

class BezierCurve : public WorldObject {
public:
    BezierCurve() : WorldObject() {}
    BezierCurve(const std::vector<Vec> &v) : WorldObject(), m_points(v) {
        evalMaxMin();
    }

    virtual void draw() const;
    virtual void drawCurve() const;

    Vec eval_arc(double arc) const;
    Vec eval_t(double t) const;

    Vec eval_deriv_arc(double arc) const;
    Vec eval_deriv_t(double t) const;

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
    mutable std::vector<double> m_cache_t;
    mutable std::vector<double> m_cache_arc;
    mutable std::vector<Vec> m_cache_pos;
    mutable std::vector<Vec> m_cache_deriv;

    std::pair<double, size_t> findInCache(double t) const;
    Vec evalCubic(Vec p0, Vec p1, Vec p2, Vec p3, double t) const;
    Vec evalCubicDeriv(Vec p0, Vec p1, Vec p2, Vec p3, double t) const;

    double m_xMin;
    double m_xMax;
    double m_zMin;
    double m_zMax;
};
