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
    ArcBallCamera(VecPolar arc) : Camera(), m_arc(arc) {}

    virtual void adjustGLU() const;
    virtual void draw() const;
    virtual void rotate(double dtheta, double dphi);

private:
    // The camera rotates around m_pos (from WoldObject)

    // Where the camera is along its motion arc. This polar vector includes
    // a radius to allow zooming.
    VecPolar m_arc;
};
