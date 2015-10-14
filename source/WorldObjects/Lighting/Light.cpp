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

    float ambient[4]
        = {(float)m_ambient.r, (float)m_ambient.g, (float)m_ambient.b};
    glLightfv(m_lightid, GL_AMBIENT, ambient);
    glChk();

    float diffuse[4]
        = {(float)m_diffuse.r, (float)m_diffuse.g, (float)m_diffuse.b};
    glLightfv(m_lightid, GL_DIFFUSE, diffuse);
    glChk();

    float specular[4]
        = {(float)m_specular.r, (float)m_specular.g, (float)m_specular.b};
    glLightfv(m_lightid, GL_SPECULAR, specular);
    glChk();
}

void Light::draw() const {
    pushMatrixAnd([&]() {
        float lpos[4] = {(float)pos().x, (float)pos().y, (float)pos().z, 1.0f};
        glLightfv(m_lightid, GL_POSITION, lpos);
        glTranslated(pos().x, pos().y, pos().z);
        glRotated(45.0, 1.0, 1.0, 1.0);
        glColor3d(1.0, 1.0, 1.0);
        glutSolidCube(0.15);
    });

    pushMatrixAnd([&]() {
        auto lookatPos = pos() + lookAt();

        glTranslated(lookatPos.x, lookatPos.y, lookatPos.z);
        glRotated(45.0, 1.0, 1.0, 1.0);
        glColor3d(1.0, 0.0, 1.0);

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
