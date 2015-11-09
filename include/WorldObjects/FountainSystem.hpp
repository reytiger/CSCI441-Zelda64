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

    float min_cone_theta = 0.0f;   // degrees
    float max_cone_theta = 360.0f; // degrees

    float min_cone_phi = 40.0f; // degrees
    float max_cone_phi = 50.0f; // degrees

    float min_speed = 20.0f; // meters
    float max_speed = 50.0f; // meters

    float min_life = 3.0f; // seconds
    float max_life = 5.0f; // seconds

    float spawn_rate = 1000.0f; // particles per second

    bool gravity = true;
};
