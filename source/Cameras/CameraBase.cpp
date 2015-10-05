#include "Cameras.hpp"

const double Camera::s_minPhi = -0.5 * M_PI + 1e-5;
const double Camera::s_maxPhi = 0.5 * M_PI - 1e-5;

void Camera::adjustGLU() const {
    glChk();
    Vec lookat = m_pos + m_arc.cart();
    // clang-format off
    gluLookAt(m_pos.x,  m_pos.y,  m_pos.z,
              lookat.x, lookat.y, lookat.z,
              m_up.x,   m_up.y,   m_up.z);
    // clang-format on
    glChk();
}

void Camera::rotate(double dtheta, double dphi) {
    m_arc.theta += dtheta;
    m_arc.phi += dphi;
    m_arc.phi = clamp(m_arc.phi, s_minPhi, s_maxPhi);
}

// Debugging the ModelView matrix can be helpful.
const float *Camera::get_modelview() const {
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

void Camera::draw() const {
    pushMatrixAnd([&]() {
        m_color.glSet();
        glTranslated(m_pos.x, m_pos.y, m_pos.z);
        glutSolidSphere(m_radius, 10, 10);
    });
}
