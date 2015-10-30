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

    void drawCurve() const;

    Vec eval_arc(float arc) const;
    Vec eval_t(float t) const;

    Vec eval_deriv_arc(float arc) const;
    Vec eval_deriv_t(float t) const;

    void loadFile(const std::string &filename);
    void evalMaxMin();

    bool drawCage = false;
    bool drawPath = false;

    float getXmin() const { return m_xMin; }
    float getXmax() const { return m_xMax; }
    float getZmin() const { return m_zMin; }
    float getZmax() const { return m_zMax; }

    void setVec(std::vector<Vec> v) { m_points = v; }
    std::vector<Vec> getVec() const { return m_points; }

    void recomputeCurve(int resolution) const;

protected:
    virtual void internalDraw() const override;

private:
    std::vector<Vec> m_points;
    // Table to cache curve points.
    mutable std::vector<float> m_cache_t;
    mutable std::vector<float> m_cache_arc;
    mutable std::vector<Vec> m_cache_pos;
    mutable std::vector<Vec> m_cache_deriv;

    std::pair<float, size_t> findInCache(float t) const;
    Vec evalCubic(Vec p0, Vec p1, Vec p2, Vec p3, float t) const;
    Vec evalCubicDeriv(Vec p0, Vec p1, Vec p2, Vec p3, float t) const;

    float m_xMin;
    float m_xMax;
    float m_zMin;
    float m_zMax;
};
