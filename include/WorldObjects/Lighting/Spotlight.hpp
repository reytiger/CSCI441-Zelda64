#pragma once
#include "Utils.hpp"

#include "WorldObjects/Lighting/Light.hpp"

class Spotlight : public Light {
public:
    Spotlight() = default;
    virtual ~Spotlight() = default;

    float exponent() const { return m_spot_exp; }
    void exponent(float exp) { m_spot_exp = exp; }

    float cutoff() const { return m_spot_cutoff; }
    void cutoff(float angle) { m_spot_cutoff = angle; }

protected:
    float m_spot_exp    = 1.0;
    float m_spot_cutoff = 45.0;

    virtual void update(double t, float dt) override;
};
