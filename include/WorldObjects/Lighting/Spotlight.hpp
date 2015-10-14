#pragma once
#include "Utils.hpp"

#include "WorldObjects/Lighting/Light.hpp"

class Spotlight : public Light {
public:
    Spotlight() = default;
    virtual ~Spotlight() = default;

    virtual void update(double t, double dt) override;

    double exponent() const { return m_spot_exp; }
    double cutoff() const { return m_spot_cutoff; }

    void exponent(double exp) { m_spot_exp = exp; }
    void cutoff(double angle) { m_spot_cutoff = angle; }

protected:
    double m_spot_exp    = 1.0;
    double m_spot_cutoff = 45.0;
};
