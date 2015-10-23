#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

class IncallidusPet : public WorldObject {
public:
    // Nothing new here.

protected:
    virtual void internalDraw() const override {
        static auto mat0 = Material::random();
        static auto mat1 = Material::random();
        static auto mat2 = Material::random();

        pushMatrixAnd([&]() {
            glTranslated(m_pos.x, m_pos.y, m_pos.z);
            glRotated(180.0 / M_PI * m_arc.theta, 0.0, 0.0, 1.0);
            glScaled(m_radius, m_radius, m_radius);

            mat0.set();
            drawBody();

            mat1.set();
            drawFrontNose();

            mat2.set();
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
