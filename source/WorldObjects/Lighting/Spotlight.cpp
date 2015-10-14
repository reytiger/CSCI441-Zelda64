#include "WorldObjects/Lighting/Spotlight.hpp"

void Spotlight::update(double t, double dt) {
    Light::update(t, dt);

    float ldir[4] = {(float)lookAt().x, (float)lookAt().y, (float)lookAt().z};
    glLightfv(m_lightid, GL_SPOT_DIRECTION, ldir);
    glChk();

    glLightf(m_lightid, GL_SPOT_EXPONENT, m_spot_exp);
    glChk();

    glLightf(m_lightid, GL_SPOT_CUTOFF, m_spot_cutoff);
    glChk();
}
