#include "Cameras/ArcBallCamera.hpp"

void ArcBallCamera::adjustGLU() {
    m_arc.r  = m_radius;
    auto pos = m_arc.cart() + m_pos;
    // clang-format off
    gluLookAt(pos.x, pos.y, pos.z,       // Where the camera is viewing from.
              m_pos.x, m_pos.y, m_pos.z, // Where the camera is 'logically'
              m_up.x, m_up.y, m_up.z);   // Which way is up?
    // clang-format on
}
