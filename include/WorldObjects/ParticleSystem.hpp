#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"
#include "Shader.hpp"

#include <vector>

template <typename Particle>
class ParticleSystem : public WorldObject {
public:
    virtual void internalDraw() const override;

    virtual void drawParticle(const Particle &particle) const = 0;

protected:
    std::vector<Particle> m_particles;

    ShaderProgram m_program;
};

template <typename Particle>
void ParticleSystem<Particle>::internalDraw() const {
    m_program.use();

    pushMatrixAnd([this]() {
        auto pos = this->pos();
        glScalef(m_radius, m_radius, m_radius);
        for (const auto &particle : this->m_particles) {
            drawParticle(particle);
        }
    });
}
