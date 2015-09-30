#pragma once
#include "Utils.hpp"

#include "Cameras/ArcBallCamera.hpp"

#include "WorldObjects.hpp"

// Skeleton to make it build.
// struct Camera : WorldObject {
struct Camera {
  Camera(Vec, Vec) {}
  void adjustGLU() {}
  void rotate(...) {}
};
