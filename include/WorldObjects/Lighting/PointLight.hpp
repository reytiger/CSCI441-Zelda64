#pragma once
#include "WorldObjects/Lighting/Light.hpp"

class PointLight : public Light {
public:
    virtual void enable() override;
};
