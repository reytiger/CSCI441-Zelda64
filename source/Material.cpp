#include "Material.hpp"

// clang-format will have a HEYDAY with this many arguments.
// clang-format off

// They're MINERALS, Marie!
const Material Material::Emerald = Material(0.0215f, 0.1745f, 0.0215f, 0.07568f, 0.61424f, 0.07568f, 0.633f, 0.727811f, 0.633f, 0.6f);
const Material Material::Jade = Material(0.135f, 0.2225f, 0.1575f, 0.54f, 0.89f, 0.63f, 0.316228f, 0.316228f, 0.316228f, 0.1f);
const Material Material::Obsidian = Material(0.05375f, 0.05f, 0.06625f, 0.18275f, 0.17f, 0.22525f, 0.332741f, 0.328634f, 0.346435f, 0.3f);
const Material Material::Pearl = Material(0.25f, 0.20725f, 0.20725f, 1.0f, 0.829f, 0.829f, 0.296648f, 0.296648f, 0.296648f, 0.088f);
const Material Material::Ruby = Material(0.1745f, 0.01175f, 0.01175f, 0.61424f, 0.04136f, 0.04136f, 0.727811f, 0.626959f, 0.626959f, 0.6f);
const Material Material::Turquoise = Material(0.1f, 0.18725f, 0.1745f, 0.396f, 0.74151f, 0.69102f, 0.297254f, 0.30829f, 0.306678f, 0.1f);
const Material Material::Brass = Material(0.329412f, 0.223529f, 0.027451f, 0.780392f, 0.568627f, 0.113725f, 0.992157f, 0.941176f, 0.807843f, 0.21794872f);
const Material Material::Bronze = Material(0.2125f, 0.1275f, 0.054f, 0.714f, 0.4284f, 0.18144f, 0.393548f, 0.271906f, 0.166721f, 0.2f);
const Material Material::Chrome = Material(0.25f, 0.25f, 0.25f, 0.4f, 0.4f, 0.4f, 0.774597f, 0.774597f, 0.774597f, 0.6f);
const Material Material::Copper = Material(0.19125f, 0.0735f, 0.0225f, 0.7038f, 0.27048f, 0.0828f, 0.256777f, 0.137622f, 0.086014f, 0.1f);
const Material Material::Gold = Material(0.24725f, 0.1995f, 0.0745f, 0.75164f, 0.60648f, 0.22648f, 0.628281f, 0.555802f, 0.366065f, 0.4f);
const Material Material::Silver = Material(0.19225f, 0.19225f, 0.19225f, 0.50754f, 0.50754f, 0.50754f, 0.508273f, 0.508273f, 0.508273f, 0.4f);

// Plastics
const Material Material::BlackPlastic = Material(0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f, 0.50f, 0.50f, 0.50f, .25f);
const Material Material::CyanPlastic = Material(0.0f, 0.1f, 0.06f, 0.0f, 0.50980392f, 0.50980392f, 0.50196078f, 0.50196078f, 0.50196078f, .25f);
const Material Material::GreenPlastic = Material(0.0f, 0.0f, 0.0f, 0.1f, 0.35f, 0.1f, 0.45f, 0.55f, 0.45f, .25f);
const Material Material::RedPlastic = Material(0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.6f, 0.6f, .25f);
const Material Material::WhitePlastic = Material(0.0f, 0.0f, 0.0f, 0.55f, 0.55f, 0.55f, 0.70f, 0.70f, 0.70f, .25f);
const Material Material::YellowPlastic = Material(0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.60f, 0.60f, 0.50f, .25f);

// Rubbers
const Material Material::BlackRubber = Material(0.02f, 0.02f, 0.02f, 0.01f, 0.01f, 0.01f, 0.4f, 0.4f, 0.4f, .078125f);
const Material Material::CyanRubber = Material(0.0f, 0.05f, 0.05f, 0.4f, 0.5f, 0.5f, 0.04f, 0.7f, 0.7f, .078125f);
const Material Material::GreenRubber = Material(0.0f, 0.05f, 0.0f, 0.4f, 0.5f, 0.4f, 0.04f, 0.7f, 0.04f, .078125f);
const Material Material::RedRubber = Material(0.05f, 0.0f, 0.0f, 0.5f, 0.4f, 0.4f, 0.7f, 0.04f, 0.04f, .078125f);
const Material Material::WhiteRubber = Material(0.05f, 0.05f, 0.05f, 0.5f, 0.5f, 0.5f, 0.7f, 0.7f, 0.7f, .078125f);
const Material Material::YellowRubber = Material(0.05f, 0.05f, 0.0f, 0.5f, 0.5f, 0.4f, 0.7f, 0.7f, 0.04f, .078125f);

// clang-format on

Material Material::random() {
    Material mat;

    mat.m_ambient   = randColor();
    mat.m_diffuse   = randColor();
    mat.m_specular  = randColor();
    mat.m_emission  = randColor();
    mat.m_shininess = 128.0f * getRand();

    return mat;
}

Material::Material(float ambient0, float ambient1, float ambient2,    //
                   float diffuse0, float diffuse1, float diffuse2,    //
                   float specular0, float specular1, float specular2, //
                   float shininess) {
    m_ambient   = Color(ambient0, ambient1, ambient2);
    m_diffuse   = Color(diffuse0, diffuse1, diffuse2);
    m_specular  = Color(specular0, specular1, specular2);
    m_shininess = shininess;
}

void Material::set() const {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission.v);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
}
