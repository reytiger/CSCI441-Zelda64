#pragma once
#include "Utils.hpp"

#include "ParticleSystem.hpp"

namespace Internal {
struct Enemy {
    Vec pos;           // meters
    Vec head;          // Direction. Used for drawing.
    float speed;       // meters / second
    float size = 1.0f; // A size of 0.0f represents a 'dead' enemy.
                       // This is also the radius of their bounding sphere.
};
};

class EnemyCrowd : public ParticleSystem<Internal::Enemy> {
public:
    using Enemy = Internal::Enemy;
    // Note that this is negative.
    static const double s_gravity; // meters / second^2

    virtual void update(double t, double dt) override;
    virtual void drawParticle(const Enemy &particle) const override;
    virtual void internalDraw() const override;

    // TODO: Is there some kind of texture release we should do?
    void tex(GLint tex) { m_tex = tex; }
    GLint tex() const { return m_tex; }

    float spawn_rate = 10.0f; // seconds per enemy

    bool gravity = true;

    WorldObject *target = nullptr;

private:
    GLint m_tex = 0;
};
