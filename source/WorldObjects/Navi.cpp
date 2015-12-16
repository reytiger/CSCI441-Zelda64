#include "WorldObjects/Navi.hpp"
#include <cmath>


Navi::Navi() {
    // not sure if this needs to be on before doing other light calls
    glEnable(GL_LIGHTING);
    m_light.enable();


    m_pos.y = 1.2f;
    m_pos.z = -0.5f;
    // setup light colors
    Color c(0.39, 0.525, 0.73);
    m_light.diffuse(c.v);
    m_light.specular(c.v);

    // attenuate so it doesn't illuinate the whole scene
    glLightf(m_light.handle(), GL_LINEAR_ATTENUATION, 0.4);

    m_scale = 0.02;

    // attach the light to Navi's position
    m_light.follow(this);

    m_material = Material::WhitePlastic;
}


void Navi::update(double t, double dt) {
    WorldObject::update(t, dt);
    moveByX(0.01f * cos(t));
    moveByZ(0.01f * sin(t));
    m_light.update(t, dt);
}


void Navi::internalDraw() const {
    glPushMatrix();
    // want to see Navi from all angles
    glDisable(GL_CULL_FACE);

    // glEnable(GL_LIGHTING);
    WorldObjModel::internalDraw();

    // since light position is tranformed by modelview matrix
    m_light.updatePosition();

    glEnable(GL_CULL_FACE);
    glPopMatrix();
}
