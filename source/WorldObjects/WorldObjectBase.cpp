#include "WorldObjects/WorldObjectBase.hpp"

void WorldObject::update(double t, double dt) {
    m_pos += dt * m_vel;
    m_pos -= m_old_follow_pos;
    if (m_update) {
        m_update(t, dt);
    }
    if (m_follow) {
        m_pos += m_follow->pos();
        m_old_follow_pos = m_follow->pos();
    }
}

void WorldObject::follow(WorldObject *wo) {
    m_follow = wo;
    m_pos -= m_old_follow_pos;
    if (m_follow) {
        m_pos += m_follow->pos();
        m_old_follow_pos = m_follow->pos();
    }
}

void WorldObject::draw() const {
    if (this->visible()) {
        this->internalDraw();
    }
}

void WorldObject::rotate(float dtheta, float dphi) {
    m_arc.theta += dtheta;
    m_arc.phi += dphi;
}

void WorldObject::doWASDControls(float speed, bool *pressed, bool use_QE) {
    Vec vel;

    Vec up      = this->up();
    Vec forward = this->lookDir().cart();
    Vec right   = forward.cross(up);

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

    // Q and E move up and down.
    if (use_QE) {
        if (pressed['q']) {
            vel += up;
        }
        if (pressed['e']) {
            vel -= up;
        }
    }

    // If no keys were pressed, vel == (0, 0) and we can't normalize.
    if (vel.norm()) {
        vel = speed * vel.normalize();
    }

    this->setVelocity(vel);
}