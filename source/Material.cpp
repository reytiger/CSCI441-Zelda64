#include "Material.hpp"

Material Material::random() {
    Material mat;

    mat.m_ambient  = randColor();
    mat.m_diffuse  = randColor();
    mat.m_specular = randColor();
    mat.m_emission = randColor();

    return mat;
}

void Material::set() const {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission.v);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
}
