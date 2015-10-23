#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

#include <functional>

class Camera : public WorldObject {
public:
    Camera() : WorldObject() { init(); }
    Camera(Vec pos, VecPolar arc) : WorldObject() {
        this->init();
        this->moveTo(pos);
        this->lookInDir(arc);
    }

    virtual void adjustGLU() const;
    virtual void rotate(double dtheta, double dphi) override {
        WorldObject::rotate(dtheta, dphi);
        m_arc.phi = clamp(m_arc.phi, -0.5 * M_PI + 1e-5, 0.5 * M_PI - 1e-5);
    }

    Vec up() const { return m_up; }

    // Debugging the ModelView matrix can be helpful.
    const float *get_modelview() const;

protected:
    virtual void internalDraw() const override;

private:
    void init() {
        this->material(Material::YellowRubber);
        this->hide(); // Don't draw cameras by default.
    }
};
