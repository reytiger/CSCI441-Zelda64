#include "WorldObjects.hpp"

GLint PointLight::s_lights = 0;

void PointLight::draw() const {
    pushMatrixAnd([&]() {
        float lpos[4] = {(float)pos().x, (float)pos().y, (float)pos().z, 1.0f};
        glLightfv(m_lightid, GL_POSITION, lpos);
        glTranslated(pos().x, pos().y, pos().z);
        glRotated(45.0, 1.0, 1.0, 1.0);
        glColor3d(1.0, 1.0, 1.0);
        glutSolidCube(0.15);
    });
}

void PointLight::ambient(const float *colorv) {
    assert(colorv != nullptr);
    assert(GL_LIGHT0 <= m_lightid);
    assert(m_lightid < GL_LIGHT7);

    m_ambient = Color(colorv[0], colorv[1], colorv[2]);

    glChk();
    glLightfv(m_lightid, GL_AMBIENT, colorv);
    glChk();
}

void PointLight::diffuse(const float *colorv) {
    assert(colorv != nullptr);
    assert(GL_LIGHT0 <= m_lightid);
    assert(m_lightid < GL_LIGHT7);

    m_diffuse = Color(colorv[0], colorv[1], colorv[2]);

    glChk();
    glLightfv(m_lightid, GL_DIFFUSE, colorv);
    glChk();
}

void PointLight::specular(const float *colorv) {
    assert(colorv != nullptr);
    assert(GL_LIGHT0 <= m_lightid);
    assert(m_lightid < GL_LIGHT7);

    m_specular = Color(colorv[0], colorv[1], colorv[2]);

    glChk();
    glLightfv(m_lightid, GL_SPECULAR, colorv);
    glChk();
}
