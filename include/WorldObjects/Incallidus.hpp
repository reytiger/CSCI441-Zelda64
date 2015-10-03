#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

class Incallidus : public WorldObject {
public:
    Incallidus() : WorldObject() {}

    virtual void draw() const {
        if (!m_visible) {
            return;
        }
        // The top of the hills are at 25.8.
        // This effectively gives color as a ratio of where you are in the map.
        auto color = m_pos / Vec(100.0, 100.0, 25.8);

        pushMatrixAnd([&]() {
            glTranslated(m_pos.x, m_pos.y, m_pos.z);
            glRotated(180.0 / M_PI * m_heading, 0.0, 0.0, 1.0);
            glScaled(m_radius, m_radius, m_radius);

            glColor3f(color.x, 0.5, 0.5);
            drawBody();

            glColor3f(0.5, color.y, 0.5);
            drawFrontNose();

            glColor3f(0.5, 0.5, color.z);
            drawBackNose();
        });
    }

private:
    void drawFrontNose() const {
        pushMatrixAnd([&]() {
            glTranslated(-1.0, 0.0, 0.0);
            glScaled(0.5, 0.5, 0.5);
            glutSolidSphere(1.0, 20, 20);
        });
    }

    void drawBackNose() const {
        pushMatrixAnd([&]() {
            glTranslated(1.0, 0.0, 0.0);
            glScaled(3.0, 0.5, 0.5);
            glutSolidSphere(1.0, 20, 20);
        });
    }

    void drawBody() const { glutSolidSphere(1.0, 20, 20); }
};
