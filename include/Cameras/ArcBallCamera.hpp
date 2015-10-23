#pragma once
#include "Utils.hpp"

#include "Cameras/CameraBase.hpp"

/*
 * Arc Ball Camera, the camera moves around a control point while mantaining
 * a radius around the point.
 *
 * This camera class assumes an x,z plane and y with the up direction.
 *
*/

/*************************  DRAW  ARC BALL CAMERA    **************************/
class ArcBallCamera : public Camera {
public:
    ArcBallCamera() : Camera() {}
    ArcBallCamera(VecPolar arc) : Camera(Vec(), arc) {}

    virtual void adjustGLU() const override;

protected:
    virtual void internalDraw() const override;

private:
};
