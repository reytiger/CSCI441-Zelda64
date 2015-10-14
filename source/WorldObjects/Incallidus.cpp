#include "WorldObjects/Incallidus.hpp"

void Incallidus::addWASDControls(double speedPerSec, bool *pressed, double dt) {
    Vec vel;

    Vec up      = this->up();
    Vec forward = this->lookAt();
    Vec right   = forward.cross(up);

    auto speed = speedPerSec * dt;

    // Basic WASD controls to move forward and sideways, *as seen by the
    // camera*.
    if (pressed['d']) {
        vel += right;
    }
    if (pressed['a']) {
        vel -= right;
    }
    if (pressed['w']) {
        vel += forward;
    }
    if (pressed['s']) {
        vel -= forward;
    }

    // If no keys were pressed, vel == (0, 0) and we can't normalize.
    if (vel.norm()) {
        vel = speed * vel.normalize();
    }

    this->setVelocity(vel);
    // this->moveTo(m_vel + m_pos);
    m_pos += m_vel;
    info("%s", m_old_follow_pos);
}