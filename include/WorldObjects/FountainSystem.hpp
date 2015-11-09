#pragma once
#include "Utils.hpp"

#include "ParticleSystem.hpp"

namespace Internal {
struct FountainParticle {
    Vec pos;               // meters
    Vec vel;               // meters / second
    float mass     = 1.0;  // g
    float lifetime = 10.0; // seconds
};
};

class FountainSystem : public ParticleSystem<Internal::FountainParticle> {
public:
    using Particle = Internal::FountainParticle;
    // Note that this is negative.
    static const double s_gravity; // meters / second^2
    static const double s_dampen;  // ???

    virtual void update(double t, double dt) override;
    virtual void drawParticle(const Particle &particle) const override;

private:
    float m_min_cone = 45.0f; // degrees
    float m_max_cone = 60.0f; // degrees

    float m_min_speed = 5.0f;   // meters
    float m_max_speed = 100.0f; // meters

    float m_min_life = 3.0f; // seconds
    float m_max_life = 5.0f; // seconds

    float m_spawn_rate = 500.0f; // particles per second
};
