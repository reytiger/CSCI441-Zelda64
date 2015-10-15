#include "Material.hpp"

// clang-format will have a HEYDAY with this many arguments.
// clang-format off

// They're MINERALS, Marie!
const Material Material::Emerald       = Material(0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6);
const Material Material::Jade          = Material(0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 0.1);
const Material Material::Obsidian      = Material(0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 0.3);
const Material Material::Pearl         = Material(0.25, 0.20725, 0.20725, 1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 0.088);
const Material Material::Ruby          = Material(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 0.6);
const Material Material::Turquoise     = Material(0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 0.1);
const Material Material::Brass         = Material(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843, 0.21794872);
const Material Material::Bronze        = Material(0.2125, 0.1275, 0.054, 0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 0.2);
const Material Material::Chrome        = Material(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6);
const Material Material::Copper        = Material(0.19125, 0.0735, 0.0225, 0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 0.1);
const Material Material::Gold          = Material(0.24725, 0.1995, 0.0745, 0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4);
const Material Material::Silver        = Material(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);

// Plastics
const Material Material::BlackPlastic  = Material(0.0, 0.0, 0.0, 0.01, 0.01, 0.01, 0.50, 0.50, 0.50, .25);
const Material Material::CyanPlastic   = Material(0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392, 0.50196078, 0.50196078, 0.50196078, .25);
const Material Material::GreenPlastic  = Material(0.0, 0.0, 0.0, 0.1, 0.35, 0.1, 0.45, 0.55, 0.45, .25);
const Material Material::RedPlastic    = Material(0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.7, 0.6, 0.6, .25);
const Material Material::WhitePlastic  = Material(0.0, 0.0, 0.0, 0.55, 0.55, 0.55, 0.70, 0.70, 0.70, .25);
const Material Material::YellowPlastic = Material(0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.60, 0.60, 0.50, .25);

// Rubbers
const Material Material::BlackRubber   = Material(0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.4, 0.4, 0.4, .078125);
const Material Material::CyanRubber    = Material(0.0, 0.05, 0.05, 0.4, 0.5, 0.5, 0.04, 0.7, 0.7, .078125);
const Material Material::GreenRubber   = Material(0.0, 0.05, 0.0, 0.4, 0.5, 0.4, 0.04, 0.7, 0.04, .078125);
const Material Material::RedRubber     = Material(0.05, 0.0, 0.0, 0.5, 0.4, 0.4, 0.7, 0.04, 0.04, .078125);
const Material Material::WhiteRubber   = Material(0.05, 0.05, 0.05, 0.5, 0.5, 0.5, 0.7, 0.7, 0.7, .078125);
const Material Material::YellowRubber  = Material(0.05, 0.05, 0.0, 0.5, 0.5, 0.4, 0.7, 0.7, 0.04, .078125);

// clang-format on

Material Material::random() {
    Material mat;

    mat.m_ambient  = randColor();
    mat.m_diffuse  = randColor();
    mat.m_specular = randColor();
    mat.m_emission = randColor();
    mat.m_shininess = 128.0 * getRandd();

    return mat;
}

Material::Material(double ambient0, double ambient1, double ambient2,    //
                   double diffuse0, double diffuse1, double diffuse2,    //
                   double specular0, double specular1, double specular2, //
                   double shininess) {
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
