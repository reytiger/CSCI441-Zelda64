
#include "WorldObjects/DragonBorn.hpp"

void DragonBorn::updateAnimation(double /*t*/, double /*dt*/) {
    // TODO: Animate Dovakiin!
}

void DragonBorn::update(double t, double dt) {
    WorldObject::update(t, dt);
    updateAnimation(t, dt);
}

/********************************  DRAW FIRNEN ********************************/
// the objects are a simple circle.
void DragonBorn::draw() const {
    if (!m_visible) {
        return;
    }
    pushMatrixAnd([&]() {
        glTranslated(m_pos.x, m_pos.y, m_pos.z);
        glColor3f(1, 0.5, 0.5);
        glutSolidSphere(1.5, 20, 20);

        auto pos = m_arc.cart() * 2;
        glTranslated(pos.x, pos.y, pos.z);
        glutSolidSphere(0.2, 20, 20);
    });
}
