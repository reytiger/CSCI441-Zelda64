#include "Cameras.hpp"

void FreeCamera::adjustGLU() {
    glChk();
    Vec lookat = m_pos + m_lookat.cart();
    // clang-format off
    gluLookAt(m_pos.x,  m_pos.y,  m_pos.z,
              lookat.x, lookat.y, lookat.z,
              m_up.x,   m_up.y,   m_up.z);
    // clang-format on
    glChk();
}

void FreeCamera::rotate(double dtheta, double dphi) {
    constexpr static double delta = 1e-5;

    m_lookat.theta += dtheta;
    m_lookat.phi += dphi;
    m_lookat.phi = clamp(m_lookat.phi, -0.5 * M_PI + delta, 0.5 * M_PI - delta);
}

// Debugging the ModelView matrix can be helpful.
const float *FreeCamera::get_modelview() const {
    // Make sure this is the camera we're loading back from OpenGL.
    static float mvm[16] = {};
    glGetFloatv(GL_MODELVIEW_MATRIX, mvm);
    // clang-format off
    info("Model View Matrix from Camera:\n"
         "%5.2f %5.2f %5.2f %5.2f\n"
         "%5.2f %5.2f %5.2f %5.2f\n"
         "%5.2f %5.2f %5.2f %5.2f\n"
         "%5.2f %5.2f %5.2f %5.2f\n"
         "\n",
         // OpenGL stores its matricies column-major.
         mvm[0], mvm[4], mvm[8],  mvm[12],
         mvm[1], mvm[5], mvm[9],  mvm[13],
         mvm[2], mvm[6], mvm[10], mvm[14],
         mvm[3], mvm[7], mvm[11], mvm[15]);
    // clang-format on
    return mvm;
}

void FreeCamera::draw() const {
    pushMatrixAnd([&]() {
        m_color.glSet();
        glTranslated(m_pos.x, m_pos.y, m_pos.z);
        glutSolidSphere(m_radius, 10, 10);
    });
}

void FreeCamera::addWASDControls(double speedPerSec, bool *pressed) {
    setUpdateFunc([=](double t, double dt) {
        Vec vel;

        Vec up      = this->up();
        Vec forward = this->lookAt();
        Vec right   = forward.cross(up);

        auto speed = dt * speedPerSec;

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
