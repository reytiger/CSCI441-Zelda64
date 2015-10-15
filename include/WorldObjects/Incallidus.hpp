#pragma once

#include "WorldObjects/WorldObjectBase.hpp"
#include "WorldObjects/WorldSurface.hpp"

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

            // draw the looking location
            glPushMatrix();
            {
                auto pos = m_arc.cart() * 2;
                glTranslated(pos.x, pos.y, pos.z);
                glutSolidSphere(0.2, 20, 20);
            };
            glPopMatrix();

            // draw their name.
            glPushMatrix();
            {
                std::string name = "Incallidus";
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

            glRotated(180.0 / M_PI * m_heading - 90, 0.0, 1.0, 0.0);
            glScaled(m_radius, m_radius, m_radius);

            glColor3d(color.x, 0.5, 0.5);
            drawBody();

            glColor3d(0.5, color.y, 0.5);
            drawFrontNose();

            glColor3d(0.5, 0.5, color.z);
            drawBackNose();

            auto pos = m_arc.cart() * 2;
            glTranslated(0, 1, 0);
            glColor3d(1, 1, 1);
            glutSolidSphere(0.2, 20, 20);
        });
    }

    void addWASDControls(double speedPerSec, bool *pressed, double dt,
                         WorldSurface world);

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
