#include "WorldObjects.hpp"

#include <algorithm>
#include <limits>

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
                continue;
            }
        }

        float dist = (p.pos - target->pos()).norm();
        if (dist < p.size + target->radius()) {
            p.size = 0.0f;
            target->radius(0.0f);
        }
    }

    // Now go through and update everything.
    for (auto &p : m_particles) {
        // Chase the hero.

        // Chase after where the hero will be in 10 frames.
        auto nextTarget = target->pos() + 10 * dt * target->vel();


        auto vel = p.speed * (nextTarget - p.pos).normalize();
        p.pos += dt * vel;
    }

    std::copy_if(m_particles.begin(),
                 m_particles.end(),
                 std::back_inserter(next_particles),
                 [](const Enemy &e) { return e.size != 0.0f; });

    // Create new particles!
    static double lastSpawn = -std::numeric_limits<double>::infinity();
    if ((t - lastSpawn) > spawn_rate) {
        lastSpawn    = t;
        // This use to spawn 20 when it got low, but that was too hard.
        size_t count = this->living() < 5 ? 17 : 5;

        for (size_t i = 0; i < count; i += 1) {

            Enemy p;
            p.size = getRand(1.5f, 2.5f);

            do {
                float theta  = getRand(2.0f * PI);
                float radius = getRand(70.0f, 100.0f);
                p.pos = target->pos() + VecPolar(theta, 0.0, radius).cart();
            } while (m_particles.end()
                     != std::find_if(m_particles.begin(),
                                     m_particles.end(),
                                     [&](const Enemy &e) {
                                         return (p.pos - e.pos).norm() < 2.0f;
                                     }));

            // VecPolar uses Radians. I lost an hour because I forgot this. ._.
            p.speed = getRand(15.0f, 20.0f);

            next_particles.push_back(p);
        }
    }

    m_particles = next_particles;
}
