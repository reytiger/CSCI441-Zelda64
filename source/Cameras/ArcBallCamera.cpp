#include "Cameras/ArcBallCamera.hpp"

void ArcBallCamera::adjustGLU() const {
    auto pos = m_arc.cart() + m_pos;
    // clang-format off
    gluLookAt(pos.x, pos.y, pos.z,       // Where the camera is viewing from
              m_pos.x, m_pos.y, m_pos.z, // Where the camera is 'logically'
              m_up.x, m_up.y, m_up.z);   // Which way is up?
    // clang-format on
}

void ArcBallCamera::draw() const {
    m_color.glSet();

    // Draw a sphere where the camera is viewing from.
    pushMatrixAnd([&]() {
        auto pos = m_arc.cart() + m_pos;
        glTranslated(pos.x, pos.y, pos.z);
        glutSolidSphere(m_radius, 10, 10);
    });

    // And a cube where it "is".
    pushMatrixAnd([&]() {
        glTranslated(m_pos.x, m_pos.y, m_pos.z);
        glutSolidCube(m_radius);
    });
}

void ArcBallCamera::rotate(double dtheta, double dphi) {
    m_arc.theta += dtheta;
    m_arc.phi = clamp(m_arc.phi + dphi, s_minPhi, s_maxPhi);
    m_arc.r   = m_radius;
}
