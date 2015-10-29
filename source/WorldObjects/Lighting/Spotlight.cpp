#include "WorldObjects/Lighting/Spotlight.hpp"

void Spotlight::update(float t, float dt) {
    Light::update(t, dt);

    auto ldir_vec = lookDir().cart();
    float ldir[4] = {(float)ldir_vec.x, (float)ldir_vec.y, (float)ldir_vec.z};
    glLightfv(m_lightid, GL_SPOT_DIRECTION, ldir);
    glChk();

    glLightf(m_lightid, GL_SPOT_EXPONENT, m_spot_exp);
    glChk();

    glLightf(m_lightid, GL_SPOT_CUTOFF, m_spot_cutoff);
    glChk();
}
