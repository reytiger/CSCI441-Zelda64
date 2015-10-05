#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

#include <functional>

class Camera : public WorldObject {
public:
    // Bounds on Phi to prevent the camera from wrapping around.
    static const double s_minPhi;
    static const double s_maxPhi;

    Camera() : WorldObject(), m_up(Vec(0.0, 1.0, 0.0)) { init(); }
    Camera(Vec pos, Vec lookat, Vec up = Vec(0.0, 1.0, 0.0))
        : WorldObject(pos), m_lookat(lookat), m_up(up) {
        init();
    }

    virtual void adjustGLU();

    virtual void draw() const;

    // TODO: Rethink the naming of this member.
    virtual Vec lookAt(VecPolar polar) { return m_lookat = polar; }
    virtual void rotate(double dtheta, double dphi);

    Vec lookAt() const { return m_lookat.cart(); }
    Vec up() const { return m_up; }

    // Debugging the ModelView matrix can be helpful.
    const float *get_modelview() const;

protected:
    VecPolar m_lookat;
    Vec m_up;

    // Set some reasonable defaults.
    void init() {
        // We want the camera to stay out of the way when it's rendered.
        m_radius = 0.05;
    }
};
