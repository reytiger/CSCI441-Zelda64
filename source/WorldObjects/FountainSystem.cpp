#include "WorldObjects.hpp"

#include <algorithm>

const double FountainSystem::s_gravity = -9.81;
const double FountainSystem::s_dampen  = 0.45;

void FountainSystem::internalDraw() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex());
    glDisable(GL_LIGHTING);

    program.use();
    glChk();

    for (const auto &particle : this->m_particles) {
        drawParticle(particle);
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void FountainSystem::drawParticle(const FountainSystem::Particle &self) const {

    // This is set in updateScene to mimic the camera.
    auto norm = up().cross(lookDir());

    // This is where I'd do the computations for billboarding, using the above norm.
    // But I mismanaged my time and am in a rush.
    // :(

    auto bl = self.pos + radius() * Vec(-1, -1);
    auto br = self.pos + radius() * Vec(+1, -1);
    auto tl = self.pos + radius() * Vec(-1, +1);
    auto tr = self.pos + radius() * Vec(+1, +1);

    auto t_bl = Vec(0.0, 0.0);
    auto t_br = Vec(1.0, 0.0);
    auto t_tl = Vec(0.0, 1.0);
    auto t_tr = Vec(1.0, 1.0);

    assert((max_life - min_life) != 0);
    float lifespan = self.lifetime / max_life;

    glBegin(GL_QUADS);

    glVertexAttrib1f(20, lifespan);
    glChk();

    glTexCoord2f(t_bl.x, t_bl.y);
    glVertex3f(bl.x, bl.y, bl.z);

    glTexCoord2f(t_br.x, t_br.y);
    glVertex3f(br.x, br.y, br.z);

    glTexCoord2f(t_tr.x, t_tr.y);
    glVertex3f(tr.x, tr.y, tr.z);

    glTexCoord2f(t_tl.x, t_tl.y);
    glVertex3f(tl.x, tl.y, tl.z);

    glEnd();
}

void FountainSystem::update(double t, double dt) {
    WorldObject::update(t, dt);

    std::vector<FountainSystem::Particle> next_particles;

    // Update existing particles, and copy them over.
    for (auto p : m_particles) {
        p.lifetime -= dt;
        if (p.lifetime <= 0) {
            continue;
        }

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

        next_particles.push_back(p);
    }

    const size_t new_m_particles = dt * spawn_rate;
    for (size_t i = 0; i < new_m_particles; i += 1) {
        FountainSystem::Particle p;

        // The center of output is the center of the FountainSystem.
        p.pos = this->pos();
        p.vel = this->vel();

        // VecPolar uses Radians. I lost an hour because I forgot this. ._.
        auto theta = PI / 180.0f * getRand(min_cone_theta, max_cone_theta);
        auto phi   = PI / 180.0f * getRand(min_cone_phi, max_cone_phi);
        auto speed = fabs(getRand(min_speed, max_speed));
        p.vel += VecPolar(theta, phi, speed).cart();

        p.lifetime = getRand(min_life, max_life);

        next_particles.push_back(p);
    }

    m_particles = next_particles;
}
