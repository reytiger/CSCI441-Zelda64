#pragma once
#include "Utils.hpp"

#include "Cameras/CameraBase.hpp"

class FirstPersonCamera : public Camera {
public:
    FirstPersonCamera() : Camera() {}
    FirstPersonCamera(VecPolar arc) : Camera(Vec(), arc) {}



    // void followLook(WorldObject *wo) {
    //     Vec target = wo->lookAt();
    //     m_arc = wo->lookAt() - m_pos; }

private:
};
