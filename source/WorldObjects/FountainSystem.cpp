#include "WorldObjects.hpp"

#include <algorithm>

const double FountainSystem::s_gravity = -9.81;
const double FountainSystem::s_dampen  = 0.85;

void FountainSystem::update(double /*t*/, double dt) {
    // TODO: Multithread!

    // Step 1: Clean out the existing points that have died.
    auto dead = std::find_if(std::begin(m_particles),
                             std::end(m_particles),
                             [&dt](const FountainSystem::Particle &p) {
                                 return p.lifetime <= as<float>(dt);
                             });

    if (dead != std::end(m_particles)) {
        dead++;
        m_particles.erase(std::begin(m_particles), dead);
    }

    // Step 2: Add new points from the fountain's 'source'.
    if (m_particles.empty()) {
        const size_t new_particles = 10000;
        for (size_t i = 0; i < new_particles; i += 1) {
            FountainSystem::Particle p;

            // Place it in a unit cube centered at (0, 0.5, 0), randomly.
            p.pos = Vec(getRand(), getRand(), getRand());
            p.pos -= Vec(0.5, 0.0, 0.5);
            p.pos *= Vec(2, 1, 2);

            p.lifetime = 1.0f * getRand();

            m_particles.push_back(p);
        }
    }

    // Step 3: Update everything.
    for (auto &p : m_particles) {
        p.lifetime -= dt;

        // If they get close to stopping, kill them.
        static const double thresh = 0.005;

        // The update order here matters.

        // Are we going to hit the floor this update?
        if ((p.pos + (dt * p.vel)).y < 0.0f) {
            // Walk forward until we hit it,
            double floortime = fabs(p.pos.y / p.vel.y);
            p.pos += floortime * p.vel;

            // If they get close to stopping, freeze them.
            // We do this here to make sure it only happens when they bounce.
            if (fabs(p.vel.y) < thresh) {
                p.pos.y = 0.0f;
                p.vel.y = 0.0f;
                continue;
            }

            // Otherwise, bounce and go back.
            p.vel.y *= -s_dampen;
            p.pos += (dt - floortime) * p.vel;
        } else {
            // Otherwise, carry on.
            p.pos += dt * p.vel;
        }

        p.vel.y += dt * s_gravity;
    }
}

void FountainSystem::drawParticle(const FountainSystem::Particle &self) const {
    pushMatrixAnd([&self]() {
        static const float radius = 5e-3; // Draw them so that their edge just
                                          // barely touches the surface.
        glTranslatef(self.pos.x, radius + self.pos.y, self.pos.z);
        glutSolidSphere(radius, 10, 10);
    });
}
