#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

#include <functional>

class FreeCamera : public WorldObject {
public:
    FreeCamera() : WorldObject(), m_up(Vec(0.0, 1.0, 0.0)) { init(); }
    FreeCamera(Vec pos, Vec lookat, Vec up = Vec(0.0, 1.0, 0.0))
        : WorldObject(pos), m_lookat(lookat), m_up(up) {
        init();
    }

    void addWASDControls(double speedPerSec, bool *pressed);

    virtual void adjustGLU();

    virtual void draw() const;
    virtual Vec lookAt(VecPolar polar) { return m_lookat = polar; }
    virtual void rotate(double dtheta, double dphi);

    Vec lookAt() const { return m_lookat; }
    Vec up() const { return m_up; }
    // Debugging the ModelView matrix can be helpful.
    const float *get_modelview() const;

protected:
    VecPolar m_lookat;
    Vec m_up;

    // Set some reasonable defaults.
    void init() { m_radius = 0.05; }
};
