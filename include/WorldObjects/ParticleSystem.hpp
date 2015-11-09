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

    size_t living() const { return m_particles.size(); }

    void clear() { m_particles.clear(); }

protected:
    ShaderProgram m_program;
    std::vector<Particle> m_particles;
};

template <typename Particle>
void ParticleSystem<Particle>::internalDraw() const {
    m_program.use();

    for (const auto &particle : this->m_particles) {
        drawParticle(particle);
    }
}
