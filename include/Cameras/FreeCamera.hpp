#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

#include <functional>

class FreeCamera : public Camera {
public:
    FreeCamera() : Camera() {}
    FreeCamera(Vec pos, VecPolar arc) : Camera(pos, arc) {}

    // Gives the camera predictable WASD controls, with Q-up and E-down too.
    void addWASDControls(double speedPerSec, bool *pressed);
};
