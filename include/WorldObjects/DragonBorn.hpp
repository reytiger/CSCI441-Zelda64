#pragma once

#include "WorldObjects/WorldObjectBase.hpp"

class DragonBorn : public WorldObject {
public:
    DragonBorn() : WorldObject() {}

    virtual void draw() const;
    virtual void update(double t, double dt);

    // modifiers

private:
    // TODO: create more drawings for a better dragonBron
    void drawTorso() const;

    void updateAnimation(double t, double dt);
};
