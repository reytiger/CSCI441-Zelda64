#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

/************************************************  DRAW FIRNEN
 * *************************************/
class Firnen : public WorldObject {
public:
    // constructors
    Firnen() : WorldObject() {
        m_armRotation = 0.0;
        m_count       = 0;
    }

    virtual void draw() const;

    // constructors
    void setArmRotation(float armRotation) { m_armRotation = armRotation; };

    void drawTorso() const;
    void drawHead() const;
    void drawEye() const;
    void drawBody() const;
    void drawArm() const;
    void drawLeg() const;

    // accessors
    float getArmRotation() { return m_armRotation; }

    // modifiers
    void updateAnimation(double t, double dt);
    int m_count;

private:
    float m_armRotation;
};