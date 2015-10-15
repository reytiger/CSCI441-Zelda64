#pragma once
#include "Utils.hpp"

class Material {
public:
    static Material random();

    ~Material() = default;
    Material() = default;

    Color ambient() const { return m_ambient; }
    Color diffuse() const { return m_diffuse; }
    Color specular() const { return m_specular; }
    Color emission() const { return m_emission; }
    double shininess() const { return m_shininess; }

    void ambient(Color ambient) { m_ambient = ambient; }
    void diffuse(Color diffuse) { m_diffuse = diffuse; }
    void specular(Color specular) { m_specular = specular; }
    void emission(Color emission) { m_emission = emission; }
    void shininess(double shininess) {
        m_shininess = clamp(shininess, 0.0, 128.0);
    }

    void set() const;

private:
    // OpenGL defaults.
    Color m_ambient    = Color(0.2, 0.2, 0.2, 1.0);
    Color m_diffuse    = Color(0.8, 0.8, 0.8, 1.0);
    Color m_specular   = Color(0.0, 0.0, 0.0, 1.0);
    Color m_emission   = Color(0.0, 0.0, 0.0, 1.0);
    double m_shininess = 0.0;
};
