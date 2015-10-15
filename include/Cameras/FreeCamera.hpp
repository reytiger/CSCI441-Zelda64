#pragma once
#include "Utils.hpp"

#include "WorldObjects.hpp"

#include <functional>

class FreeCamera : public Camera {
public:
    FreeCamera() : Camera() {}
    FreeCamera(Vec pos, VecPolar arc, Vec up = Vec(0.0, 1.0, 0.0))
        : Camera(pos, arc, up) {}

    // Gives the camera predictable WASD controls, with Q-up and E-down too.
    void addWASDControls(double speedPerSec, bool *pressed);
};
