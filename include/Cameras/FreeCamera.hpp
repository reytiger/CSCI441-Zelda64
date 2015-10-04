#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

class FreeCamera : public WorldObject {
public:
    FreeCamera() : WorldObject(), m_up(Vec(0.0, 1.0, 0.0)) {}
    FreeCamera(Vec pos, Vec lookat, Vec up = Vec(0.0, 1.0, 0.0))
        : WorldObject(pos), m_lookat(lookat), m_up(up) {
        m_radius = 0.05;
    }

    virtual void adjustGLU();

    virtual void draw() const;
    virtual Vec lookAt(VecPolar polar) { return m_lookat = polar; }
    virtual void rotate(double dtheta, double dphi);

    Vec lookAt() const { return m_lookat; }

    // Debugging the ModelView matrix can be helpful.
    const float *get_modelview() const;

protected:
    VecPolar m_lookat;
    Vec m_up;
};
