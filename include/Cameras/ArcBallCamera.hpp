#pragma once
#include "Utils.hpp"

#include <stdio.h>
#include <math.h>
#include <iostream>

/*
 * Arc Ball Camera, the camera moves around a control point while mantaining
 * a radius around the point.
 *
 * This camera class assumes an x,z plane and y with the up direction.
 *
*/

/*************************  DRAW  ARC BALL CAMERA    **************************/
class ArcBallCamera {
public:
  ///  CONSTRUCTORS  ///
  ArcBallCamera();

  void setX(float x) { this->cameraX = x; };
  void setY(float y) { this->cameraY = y; };
  void setZ(float z) { this->cameraZ = z; };
  void setDirX(float dirX) { this->dirX = dirX; };
  void setDirY(float dirY) { this->dirY = dirY; };
  void setDirZ(float dirZ) { this->dirZ = dirZ; };
  void setTheta(float theta) { this->theta = theta; };
  void setPhi(float phi) { this->phi = phi; };

  void setU(float u) { this->u = u; };
  void setV(float v) { this->v = v; };

  void setView(float x, float y, float z);
  void setScale(float scale) { this->scale = scale; };
  void setAngle(float angle) { this->angle = angle; };

  void setXY(float x, float y) {
    this->cameraX = x;
    this->cameraY = y;
  };
  void setXYZ(float x, float y, float z);

  void drawCamera();

  ///  ACCESSORS  ///
  float getX() { return this->cameraX; }
  float getY() { return this->cameraY; }
  float getZ() { return this->cameraZ; }
  float getDirX() { return this->dirX; };
  float getDirY() { return this->dirY; };
  float getDirZ() { return this->dirZ; };
  float getTheta() { return this->theta; };
  float getPhi() { return this->phi; };

  float getScale() { return this->scale; }
  float getAngle() { return this->angle; }

  float getU() { return u; }
  float getV() { return v; }

  void print();

  ///  MODIFIERS  ///
  void recomputeOrientation();

private:
  float cameraX, cameraY, cameraZ, dirX, dirY, dirZ, theta, phi;
  float u, v;
  float scale, angle;
  float viewX, viewY, viewZ; // used for keeping track of the origin
};
