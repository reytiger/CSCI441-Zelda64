#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

/*************************  DRAW FIRNENS CART  ********************************/
class FirnensCart : public WorldObject {
public:
    // constructors
    FirnensCart() : WorldObject() { m_wheelAngle = 0.0; }

    virtual void draw() const;

    void setAngleWheel(float angle) { this->m_wheelAngle = angle; };

    // accessors
    float getAngleWheel() { return this->m_wheelAngle; }

private:
    float m_wheelAngle;
};