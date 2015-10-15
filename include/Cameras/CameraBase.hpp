#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

#include <functional>

class Camera : public WorldObject {
public:
    // Bounds on Phi to prevent the camera from wrapping around.
    static const double s_minPhi;
    static const double s_maxPhi;

    Camera() : WorldObject(), m_up(Vec(0.0, 1.0, 0.0)) {
        m_radius   = 0.05;
        m_material = Material::YellowRubber;
    }
    Camera(Vec pos, Vec arc, Vec up = Vec(0.0, 1.0, 0.0))
        : WorldObject(pos), m_up(up) {
        m_arc      = arc;
        m_radius   = 0.05;
        m_material = Material::YellowRubber;
    }

    virtual void adjustGLU() const;

    virtual void draw() const;

    // TODO: Rethink the naming of this member.
    virtual void rotate(double dtheta, double dphi);

    Vec up() const { return m_up; }

    // Debugging the ModelView matrix can be helpful.
    const float *get_modelview() const;

protected:
    Vec m_up;
};
