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
    void setCurvesCPoints(std::vector<Vec> v) {
        m_halo.setVec(v);
        m_halo.recomputeCurve(10000);
    }

    Vec eval_t(double t);
    Vec eval_arc(double arc);
    Vec eval_deriv_t(double t);
    Vec eval_deriv_arc(double arc);

    double evalAngle(double t); // returns 0-360;

    virtual void draw() const;

private:
    // set up for control points
    std::vector<BezierCurve> m_curvesCPoints;

    // TODO: update after calllist works
    void drawTrackBlocks() const;
    CallListObject m_trackCallBack;

    BezierCurve m_halo;
};
