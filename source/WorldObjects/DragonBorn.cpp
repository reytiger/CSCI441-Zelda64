
#include "WorldObjects/DragonBorn.hpp"


void DragonBorn::updateAnimation(double t, double dt) {}

/********************************  DRAW FIRNEN ********************************/
// the objects are a simple circle.
void DragonBorn::draw() const {
    if (!m_visible) {
        return;
    }
    pushMatrixAnd([&]() {
        glTranslated(m_pos.x, m_pos.y, m_pos.z);
        randColor().glSet();
        glutSolidSphere(1.5, 20, 20);
    });
}
