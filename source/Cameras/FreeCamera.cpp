#include "Cameras.hpp"

void FreeCamera::adjustGLU() {
    glChk();
    Vec lookat = m_pos + m_lookat.cart();
    gluLookAt(m_pos.x, // Position
              m_pos.y,
              m_pos.z,
              lookat.x, // Look at
              lookat.y,
              lookat.z,
              m_up.x, // Up
              m_up.y,
              m_up.z);
    glChk();
}

void FreeCamera::rotate(double dtheta, double dphi) {
    m_lookat.theta += dtheta;
    m_lookat.phi += dphi;
    // Because we use positive Z as up, our bounds on PHI change.
    m_lookat.phi
        = clamp(m_lookat.phi, 0.5 * M_PI + 0.0001, 1.5 * M_PI + 0.0001);
}

// Debugging the ModelView matrix can be helpful.
const float *FreeCamera::get_modelview() const {
    // Make sure this is the camera we're loading back from OpenGL.
    static float mvm[16] = {};
    glGetFloatv(GL_MODELVIEW_MATRIX, mvm);
    fprintf(stderr, //
            "Model View Matrix from Camera:\n"
            "%5.2f %5.2f %5.2f %5.2f\n"
            "%5.2f %5.2f %5.2f %5.2f\n"
            "%5.2f %5.2f %5.2f %5.2f\n"
            "%5.2f %5.2f %5.2f %5.2f\n"
            "\n",
            mvm[0],
            mvm[1],
            mvm[2],
            mvm[3],
            mvm[4],
            mvm[5],
            mvm[6],
            mvm[7],
            mvm[8],
            mvm[9],
            mvm[10],
            mvm[11],
            mvm[12],
            mvm[13],
            mvm[14],
            mvm[15]);
    return mvm;
}

void FreeCamera::draw() const {
    pushMatrixAnd([&]() {
        glTranslated(m_pos.x, m_pos.y, m_pos.z);
        glutSolidSphere(m_radius, 10, 10);
    });
}
