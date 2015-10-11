
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

        // draw their name.
        glPushMatrix();
        {
            std::string name = "Ulfric Stormcloak";
            glDisable(GL_LIGHTING);
            glTranslated(-(int)name.size() / 4, 3, 0);
            glScaled(0.01, 0.01, 0.01);
            glColor3d(0.9, 0.2, 0.15);
            for (char c : name) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
            }
            glEnable(GL_LIGHTING);
        };
        glPopMatrix();

        glColor3f(1, 0.5, 0.5);
        glutSolidSphere(1.5, 20, 20);

        auto pos = m_arc.cart() * 2;
        glTranslated(pos.x, pos.y, pos.z);
        glutSolidSphere(0.2, 20, 20);
    });
}
