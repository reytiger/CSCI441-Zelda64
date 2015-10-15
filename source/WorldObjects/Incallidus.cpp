#include "WorldObjects/Incallidus.hpp"

void Incallidus::addWASDControls(double speedPerSec, bool *pressed, double dt,
                                 WorldSurface world) {
    Vec vel;

    Vec up      = this->up();
    Vec forward = VecPolar(m_heading, 0, 1);

    auto speed = speedPerSec * dt;

    // Basic WASD controls to move forward and sideways, *as seen by the
    // camera*.
    if (pressed['d']) {
        m_heading += 0.1;
    }
    if (pressed['a']) {
        m_heading -= 0.1;
    }
    if (pressed['w']) {
        vel -= forward;
    }
    if (pressed['s']) {
        vel += forward;
    }

    vel.y = 0.0;

    // If no keys were pressed, vel == (0, 0) and we can't normalize.
    if (vel.norm()) {
        vel = speed * vel.normalize();
    }

    m_pos      = world.eval(m_pos.x, m_pos.z);
    Vec tarPos = world.eval(vel.x + m_pos.x, vel.z + m_pos.z);

    // info("%s\n%s", m_pos, tarPos);

    vel = m_pos - tarPos;

    this->setVelocity(vel);
}