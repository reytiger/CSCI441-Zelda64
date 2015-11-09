#include "WorldObjects.hpp"

#include <algorithm>

const double FountainSystem::s_gravity = -9.81;
const double FountainSystem::s_dampen  = 0.68;

void FountainSystem::internalDraw() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex());

    auto mat = this->material();
    mat.emission(Color(1.0, 0.84, 0.0));
    mat.set();
    ParticleSystem<Particle>::internalDraw();

    glDisable(GL_TEXTURE_2D);
}

void FountainSystem::drawParticle(const FountainSystem::Particle &self) const {
    pushMatrixAnd([&self, this]() {
        glTranslatef(self.pos.x, self.pos.y, self.pos.z);

        assert((max_life - min_life) != 0);
        // The 'radius' of the billboarded quad.
        float halfsize = (self.lifetime - min_life) / (max_life - min_life);

        // The 'radius' of the area sampled from the texture.
        float texsize = 0.1;

        // TODO: Make these Vec(+1, +1) into billboarding vectors!
        auto bl = self.pos + halfsize * Vec(-1, -1);
        auto br = self.pos + halfsize * Vec(+1, -1);
        auto tl = self.pos + halfsize * Vec(-1, +1);
        auto tr = self.pos + halfsize * Vec(+1, +1);

        auto t_bl = Vec(0.0, 0.0);
        auto t_br = Vec(1.0, 0.0);
        auto t_tl = Vec(0.0, 1.0);
        auto t_tr = Vec(1.0, 1.0);

        glBegin(GL_QUADS);

        glTexCoord2f(t_bl.x, t_bl.y);
        glVertex4f(bl.x, bl.y, bl.z, 0.5);

        glTexCoord2f(t_br.x, t_br.y);
        glVertex4f(br.x, br.y, br.z, 0.5);

        glTexCoord2f(t_tr.x, t_tr.y);
        glVertex4f(tr.x, tr.y, tr.z, 0.5);

        glTexCoord2f(t_tl.x, t_tl.y);
        glVertex4f(tl.x, tl.y, tl.z, 0.5);

        glEnd();
    });
}

void FountainSystem::update(double t, double dt) {
    WorldObject::update(t, dt);
    // Step 1: Clean out the existing points that have died.

    // Order them by first-to-die.
    std::sort(std::begin(m_particles),
              std::end(m_particles),
              [&dt](const FountainSystem::Particle &lhs,
                    const FountainSystem::Particle &rhs) {
                  return lhs.lifetime < rhs.lifetime;
              });

    // And kill them.
    auto dead = std::find_if(m_particles.begin(),
                             m_particles.end(),
                             [&dt](const FountainSystem::Particle &p) {
                                 return p.lifetime > as<float>(dt);
                             });
    if (dead != m_particles.end()) {
        m_particles.erase(std::begin(m_particles), dead);
    }

    // Step 2: Add new points from the fountain's 'source'.
    // if (m_particles.empty()) {
    {
        const size_t new_m_particles = dt * spawn_rate;
        for (size_t i = 0; i < new_m_particles; i += 1) {
            FountainSystem::Particle p;

            // The center of output is the center of the FountainSystem.
            p.pos = this->pos();
            p.vel = this->vel();

            p.uv = Vec(getRand(), getRand());

            // VecPolar uses Radians. I lost an hour because I forgot this. ._.
            auto theta = PI / 180.0f * getRand(min_cone_theta, max_cone_theta);
            auto phi   = PI / 180.0f * getRand(min_cone_phi, max_cone_phi);
            auto speed = fabs(getRand(min_speed, max_speed));
            p.vel += VecPolar(theta, phi, speed).cart();

            p.lifetime = getRand(min_life, max_life);

            m_particles.push_back(p);
        }
    }

    // Step 3: Update everything.
    for (auto &p : m_particles) {
        p.lifetime -= dt;

        // Are we going to hit the floor this update?
        if ((p.pos + (dt * p.vel)).y < 0.0f) {
            // Walk forward until we hit it...
            double floortime = fabs(p.pos.y / p.vel.y);
            p.pos += floortime * p.vel;

            // ...then bounce and go back.
            p.vel.y *= -s_dampen;
            p.pos += (dt - floortime) * p.vel;
        } else {
            // Otherwise, carry on.
            p.pos += dt * p.vel;
        }

        if (gravity) {
            p.vel.y += dt * s_gravity;
        }
    }
}
