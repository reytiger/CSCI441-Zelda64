#include "Cameras/ArcBallCamera.hpp"

#include <stdio.h>
#include <math.h>
#include <iostream>

///  CONSTRUCTORS  ///
ArcBallCamera::ArcBallCamera() {
  cameraX = 0.0;
  cameraY = 0.0;
  cameraZ = 0.0;
  dirX = 0.0;
  dirY = 0.0;
  dirZ = 0.0;

  phi = 0.0;
  theta = 0.0;

  scale = 0.0;
  angle = 0.0;
}

void ArcBallCamera::setView(float x, float y, float z) {
  this->viewX = x;
  this->viewY = y;
  this->viewZ = z;
}

void ArcBallCamera::setXYZ(float x, float y, float z) {
  this->cameraX = x;
  this->cameraY = y;
  this->cameraZ = z;
}

/****************************  ARC BALL CAMER    ******************************/
void ArcBallCamera::drawCamera() {
  gluLookAt(cameraX, cameraY, cameraZ, viewX, viewY, viewZ, 0, 1,
            0); // up vector is (0,1,0) (positive Y)
}

////////////////////////////////////////////////////////////////////////////////
// This function updates the camera's position in cartesian coordinates based
//  on its position in spherical coordinates. Should be called every time
//  cameraTheta, cameraPhi, or cameraRadius is updated.
void ArcBallCamera::recomputeOrientation() {
  // testing, the camear moves up....
  float newX, newY, newZ;
  float oldX = cameraX, oldY = cameraY, oldZ = cameraZ;

  float cameraRadius =
      sqrt(pow((viewX - cameraX), 2) + pow((viewY - cameraY), 2) +
           pow((viewZ - cameraZ), 2));
  cameraX = cameraRadius * sin(theta) * sin(phi);
  cameraY = -cameraRadius * cos(phi);
  cameraZ = -cameraRadius * cos(theta) * sin(phi);

  cameraX += viewX;
  cameraY += viewY;
  cameraZ += viewZ;

  dirX = cameraRadius * sin(theta) * sin(phi);
  dirY = -cameraRadius * cos(phi);
  dirZ = -cameraRadius * cos(theta) * sin(phi);

  // and NORMALIZE this directional vector!!!
  float length = sqrt((dirX * dirX) + (dirY * dirY) + (dirZ * dirZ));
  dirX = dirX / length;
  dirY = dirY / length;
  dirZ = dirZ / length;
}

void ArcBallCamera::print() {
  std::cout << "Camera: " << cameraX << "  " << cameraY << "  " << cameraZ
            << std::endl;
  std::cout << "dir:    " << dirX << "  " << dirY << "  " << dirZ << std::endl;
  std::cout << "Pitch (phi): " << phi << "   Yaw (Theat):  " << theta
            << std::endl;
  std::cout << "looking at: " << viewX << " " << viewY << " " << viewZ
            << std::endl;
  std::cout << std::endl;
}
