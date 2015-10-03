#pragma once
#include "Utils.hpp"

#include "Cameras.hpp"
#include "WorldObjects.hpp"

#include <vector>

namespace PrettyGLUT {

// Display settings.
// For more detailed settings, see initGLUT in PrettyGLUT.cpp.
constexpr const char *windowTitle = "Skyrim belongs to the Nords!";

constexpr double FPS = 30.0;
constexpr double FOV = 60.0;

extern int windowWidth;
extern int windowHeight;
static inline double aspectRatio() {
  return (double)windowWidth / windowHeight;
}

extern Color colorClear;

// Input states
extern Vec mouse;
extern int leftMouse;
extern bool keyPressed[256];

// "public" functions
void printOpenGLInformation();
void initGLUT(int *argcp, char **argv);
void start();

// Things to draw
extern std::vector<WorldObject *> drawn;

}; // namespace PrettyGLUT

// Tree settings
extern double treeGirth;

// Hero settings
extern double heroBankRate;
extern double heroWalkSpeed;

// City settings
constexpr Vec citySize = Vec(100.0, 100.0, 10.0);
constexpr int cityResolution = 100;

enum CameraMode {
  ThirdPerson, // kind of
  FirstPerson,
};

extern Camera defaultCamera;
extern CameraMode cameraMode;
