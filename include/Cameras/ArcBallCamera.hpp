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

    virtual void adjustGLU();
    virtual void rotate(double dtheta, double dphi) {
        m_arc.theta += dtheta;
        m_arc.phi = clamp(m_arc.phi + dphi, s_minPhi, s_maxPhi);
        m_arc.r   = m_radius;
    }

private:
    float cameraX, cameraY, cameraZ, dirX, dirY, dirZ, theta, phi;
    float u, v;
    float scale, angle;
    float viewX, viewY, viewZ; // used for keeping track of the origin

    // The camera rotates around m_pos (from WoldObject)

    // Where the camera is along its motion arc. This polar vector includes
    // a radius to allow zooming.
    VecPolar m_arc;
};
