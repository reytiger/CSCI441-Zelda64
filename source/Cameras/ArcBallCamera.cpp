#include "Cameras/ArcBallCamera.hpp"

void ArcBallCamera::adjustGLU() const {
    auto pos = m_arc.cart() * m_radius + m_pos;
    // clang-format off
    gluLookAt(pos.x, pos.y, pos.z,       // Where the camera is viewing from
              m_pos.x, m_pos.y, m_pos.z, // Where the camera is 'logically'
              m_up.x, m_up.y, m_up.z);   // Which way is up?
    // clang-format on
}

void ArcBallCamera::internalDraw() const {
    if (!m_visible) {
        return;
    }
    m_material.set();

    // Draw a sphere where the camera is viewing from.
    pushMatrixAnd([&]() {
        auto pos = m_arc.cart() + m_pos;
        glTranslated(pos.x, pos.y, pos.z);
        glutSolidSphere(0.1, 10, 10);
    });

    if (kDrawLookAtTargets) {
        // And a cube where it "is".
        pushMatrixAnd([&]() {
            glTranslated(m_pos.x, m_pos.y, m_pos.z);
            glutSolidCube(0.1);
        });
    }
}
