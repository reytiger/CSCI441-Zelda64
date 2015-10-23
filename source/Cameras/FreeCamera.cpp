#include "Cameras.hpp"

void FreeCamera::addWASDControls(double speed, bool *pressed) {
    setUpdateFunc([=](double /*t*/, double /*dt*/) {
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
        if (pressed['q']) {
            vel += up;
        }
        if (pressed['e']) {
            vel -= up;
        }

        // If no keys were pressed, vel == (0, 0) and we can't normalize.
        if (vel.norm()) {
            vel = speed * vel.normalize();
        }

        this->setVelocity(vel);
    });
}
