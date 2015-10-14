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
    Vec target = world.eval(m_pos.x, m_pos.z);

    this->moveToY(target.y);

    // if (m_pos.x < 0) {
    //     target = Vec(-m_pos.x, target.y, m_pos.z);

    // }
    // // pos
    // else {
    //     target = Vec(m_pos.x, target.y, m_pos.z);
    // }
}