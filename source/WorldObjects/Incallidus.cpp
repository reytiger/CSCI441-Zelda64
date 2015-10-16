#include "WorldObjects/Incallidus.hpp"

void Incallidus::addWASDControls(double speedPerSec, bool *pressed, double dt,
                                 WorldSurface world) {
    Vec vel;

    Vec up      = this->up();
    Vec forward = VecPolar(m_heading, 0, 1);

    this->lookInDir(forward);

    auto speed = speedPerSec * dt;

    // Basic WASD controls to move forward and sideways, *as seen by the
    // camera*.
    if (pressed['d']) {
        m_heading -= 0.1;
    }
    if (pressed['a']) {
        m_heading += 0.1;
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

    // Make him float above the surface *just barely*.
    auto floating = Vec(0.0, 0.5, 0.0);

    m_pos      = world.eval(m_pos.x, m_pos.z) + floating;
    Vec tarPos = world.eval(vel.x + m_pos.x, vel.z + m_pos.z) + floating;

    vel = m_pos - tarPos;

    if (vel.norm()) {
        vel = speed * vel.normalize();
    }


    this->setVelocity(vel);
}
