#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"
#include "Shader.hpp"

#include <vector>

template <typename Particle>
class ParticleSystem : public WorldObject {
public:
    virtual void internalDraw() const override = 0;

    virtual void drawParticle(const Particle &particle) const = 0;

    size_t living() const { return m_particles.size(); }

    void clear() { m_particles.clear(); }

    ShaderProgram program;

protected:
    std::vector<Particle> m_particles;
};
