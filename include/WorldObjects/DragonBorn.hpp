#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

class DragonBorn : public WorldObject {
public:
    DragonBorn() : WorldObject() {}

    virtual void draw() const;

    // modifiers
    void updateAnimation(double t, double dt);

private:
    // TODO: create more drawings for a better dragonBron
    void drawTorso() const;
};
