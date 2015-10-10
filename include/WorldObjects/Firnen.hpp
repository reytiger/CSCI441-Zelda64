#pragma once

#include "WorldObjects/WorldObjectBase.hpp"
#include "WorldObjects/FirnensAkkkin.hpp"

/************************************************  DRAW FIRNEN
 * *************************************/
class Firnen : public WorldObject {
public:
    // constructors
    Firnen() = default;

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

    void load() {
        m_akkin.loadControlPoints("assets/world/AkkkinControlPoints.csv");
    }

private:
    FirnensAkkkin m_akkin;
    float m_armRotation = 0.0f;
};