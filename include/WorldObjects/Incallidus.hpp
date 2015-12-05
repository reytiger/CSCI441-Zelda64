#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

class Incallidus : public WorldObject {
public:
    Incallidus() : WorldObject() {}

protected:
    virtual void internalDraw() const override {
        pushMatrixAnd([&]() {
            glTranslated(m_pos.x, m_pos.y, m_pos.z);

            // draw their name.
            pushMatrixAnd([&]() {
                glDisable(GL_LIGHTING);

                static const std::string name = "Incallidus";

                auto offset = -as<int>(name.size()) / 4.0f;
                glTranslatef(offset, 3.0f, 0.0f);
                glScalef(0.01f, 0.01f, 0.01f);

                glColor3f(1.0f, 1.0f, 1.0f);
                for (auto c : name) {
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
                }

                glEnable(GL_LIGHTING);
            });

            // Draw the body parts.
            pushMatrixAnd([&]() {
                glRotated(180.0 / M_PI * m_arc.theta - 90, 0.0, 1.0, 0.0);
                glScaled(m_radius, m_radius, m_radius);

                drawBody();

                drawFrontNose();

                drawBackNose();

                Vec pos = m_arc.cart() * 2;
                glTranslated(pos.x, pos.y, pos.z);
                glutSolidSphere(0.2, 20, 20);
            });
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
