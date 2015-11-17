#include "WorldObjects.hpp"

#include <algorithm>

const double EnemyCrowd::s_gravity = -9.81;

using Enemy = EnemyCrowd::Enemy;

void EnemyCrowd::internalDraw() const {
    glChk();
    for (const auto &particle : this->m_particles) {
        glChk();
        drawParticle(particle);
        glChk();
    }
}

void EnemyCrowd::drawParticle(const Enemy &self) const {
    pushMatrixAnd([&]() {
        glTranslatef(self.pos.x, self.pos.y, self.pos.z);
        glutSolidCube(2.0f * self.size);
    });
}

void EnemyCrowd::update(double t, double dt) {
    WorldObject::update(t, dt);

    assert(target != nullptr);

    std::vector<Enemy> next_particles;

    // Collision detection.
    for (auto &p : m_particles) {
        for (auto &o : m_particles) {
            if (&p == &o) {
                continue;
            }

            // Are they touching now?
            float dist = (p.pos - o.pos).norm();
            if (dist < p.size + o.size) {
                // collide! Note: A size of 0.0f represents a 'dead' enemy.
                p.size = 0.0f;
                o.size = 0.0f;
            }
        }
    }

    // Now go through and update everything.
    for (auto &p : m_particles) {
        // Chase the hero.
        auto vel = p.speed * (target->pos() - p.pos).normalize();
        p.pos += vel;
    }

    std::copy_if(m_particles.begin(),
                 m_particles.end(),
                 std::back_inserter(next_particles),
                 [](const Enemy &e) { return e.size != 0.0f; });

    // Create new particles!
    // const size_t new_particle_count = dt * spawn_rate;
    const size_t new_particle_count = 1;
    for (size_t i = 0; i < new_particle_count; i += 1) {
        Enemy p;

        p.size = getRand(0.5f, 1.5f);

        float area = 50.0f;
        p.pos      = Vec(getRand(-area, area), 1.5, getRand(-area, area));

        // VecPolar uses Radians. I lost an hour because I forgot this. ._.
        p.speed = getRand(0.1f);

        next_particles.push_back(p);
    }

    m_particles = next_particles;
}
