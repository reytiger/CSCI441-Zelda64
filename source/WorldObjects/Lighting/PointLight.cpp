#include "WorldObjects.hpp"

void PointLight::enable() {
    m_lightid = GL_LIGHT0 + s_lights;
    s_lights += 1;
    assert(s_lights < 8);

    glEnable(m_lightid);

    // TODO: Make Color a Vec and add a "to-v" function or something.
    float lpos[4] = {(float)pos().x, (float)pos().y, (float)pos().z, 1.0f};
    glLightfv(m_lightid, GL_POSITION, lpos);
}
