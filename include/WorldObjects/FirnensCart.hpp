#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

/*************************  DRAW FIRNENS CART  ********************************/
class FirnensCart : public WorldObject {
public:
  // constructors
  Cart() : WorldObject() { m_wheelAngle = 0.0; }

  virtual void draw() const;

  void setAngleWheel(float angle) { this->m_angleWheel = angle; };

  // accessors
  float getAngleWheel() { return this->m_angleWheel; }

private:
  float m_angleWheel;
};