#include "WorldObjects.hpp"

GLint Light::s_lights = 0;

void Light::enable() {
    m_lightid = GL_LIGHT0 + s_lights;
    s_lights += 1;
    assert(s_lights <= 8);

    glEnable(m_lightid);
}

void Light::update(double t, double dt) {
    WorldObject::update(t, dt);
    glChk();

    float lpos[4] = {(float)pos().x, (float)pos().y, (float)pos().z};
    glLightfv(m_lightid, GL_POSITION, lpos);
    glChk();

    glLightfv(m_lightid, GL_AMBIENT, m_ambient.v);
    glChk();

    glLightfv(m_lightid, GL_DIFFUSE, m_diffuse.v);
    glChk();

    glLightfv(m_lightid, GL_SPECULAR, m_specular.v);
    glChk();
}

void Light::draw() const {
    auto mat = Material::WhiteRubber;
    mat.emission(m_diffuse);

    pushMatrixAnd([&]() {
        float lpos[4] = {(float)pos().x, (float)pos().y, (float)pos().z, 1.0f};
        glLightfv(m_lightid, GL_POSITION, lpos);

        glTranslated(pos().x, pos().y, pos().z);
        glRotated(45.0, 1.0, 1.0, 1.0);
        glutSolidCube(0.15);
    });

    pushMatrixAnd([&]() {
		auto lookTarget = pos() + lookDir();
        glTranslated(lookTarget.x, lookTarget.y, lookTarget.z);
        glRotated(45.0, 1.0, 1.0, 1.0);
        glutSolidCube(0.075);
    });
}

void Light::ambient(const float *colorv) {
    assert(colorv != nullptr);
    assert(GL_LIGHT0 <= m_lightid);
    assert(m_lightid <= GL_LIGHT7);

    m_ambient = Color(colorv[0], colorv[1], colorv[2]);
}

void Light::diffuse(const float *colorv) {
    assert(colorv != nullptr);
    assert(GL_LIGHT0 <= m_lightid);
    assert(m_lightid <= GL_LIGHT7);

    m_diffuse = Color(colorv[0], colorv[1], colorv[2]);
}

void Light::specular(const float *colorv) {
    assert(colorv != nullptr);
    assert(GL_LIGHT0 <= m_lightid);
    assert(m_lightid <= GL_LIGHT7);

    m_specular = Color(colorv[0], colorv[1], colorv[2]);
}
