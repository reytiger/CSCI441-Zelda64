#pragma once
#include "Utils.hpp"

class Material {
public:
    // Create a material where each component is a random color.
    // This includes emission.
    static Material random();

    // Predefined materials from:
    //    http://devernay.free.fr/cours/opengl/materials.html
    static const Material Emerald;
    static const Material Jade;
    static const Material Obsidian;
    static const Material Pearl;
    static const Material Ruby;
    static const Material Turquoise;
    static const Material Brass;
    static const Material Bronze;
    static const Material Chrome;
    static const Material Copper;
    static const Material Gold;
    static const Material Silver;

    static const Material BlackPlastic;
    static const Material CyanPlastic;
    static const Material GreenPlastic;
    static const Material RedPlastic;
    static const Material WhitePlastic;
    static const Material YellowPlastic;

    static const Material BlackRubber;
    static const Material CyanRubber;
    static const Material GreenRubber;
    static const Material RedRubber;
    static const Material WhiteRubber;
    static const Material YellowRubber;

    ~Material() = default;
    Material() = default;

    // 10 doubles. 3 3-component colors + shininess.
    Material(double, double, double, double, double, double, double, double,
             double, double);

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
