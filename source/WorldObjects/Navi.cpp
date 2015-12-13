#include "WorldObjects/Navi.hpp"
#include <cmath>


Navi::Navi() {
    // not sure if this needs to be on before doing other light calls
    glEnable(GL_LIGHTING);
    m_light.enable();

    m_pos.y = 10.f;
    // setup light colors
    Color c(0.39, 0.525, 0.73);
    m_light.diffuse(&c.v[0]);
    m_light.ambient(&c.v[0]);
    m_light.specular(&c.v[0]);

    // attach the light to Navi's position
    m_light.follow(this);

    m_material = Material::WhitePlastic;
    // update function
    auto f = [&](double t, double dt) {
        moveByX(0.1 * cos(t));
        moveByZ(0.1 * sin(t));
        m_light.update(t, dt);
    };
    setUpdateFunc(f);
}


void Navi::internalDraw() const {
    glPushMatrix();
    // want to see Navi from all angles
    glDisable(GL_CULL_FACE);
    glScalef(0.1, 0.1, 0.1);

    glEnable(GL_LIGHTING);
    WorldObjModel::internalDraw();
    m_light.draw();

    glEnable(GL_CULL_FACE);
    glPopMatrix();
}
