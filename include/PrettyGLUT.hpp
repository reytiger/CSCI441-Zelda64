#pragma once
#include "Utils.hpp"

#include "Cameras.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "WorldObjects.hpp"

#include "ModelLoader.hpp"

#include <vector>

// Things to draw
// Pointers stored here can expect to live the duration of the program.
extern std::vector<WorldObject *> drawn;

// Cameras
extern FreeCamera freecam;

extern Camera *activeCam;

extern double live_fps;
extern double live_frametime;
extern int live_frames;

// Display settings.
// For more detailed settings, see initGLUT in PrettyGLUT.cpp.
constexpr const char *windowTitle = "A little green tunic.";

constexpr float FOV = 60.0;

// This is really a limit on updating the scene. It's considerably higher than
// the actual frame rate.
constexpr size_t FPS = 300;
// How often is the HUD display for frame rate/time/etc updated?
constexpr auto FPS_update_delay = std::chrono::seconds(1);

extern int windowWidth;
extern int windowHeight;
static inline float aspectRatio() { return (float)windowWidth / windowHeight; }

extern Color colorClear;

// Input states
extern Vec mouse;
extern int leftMouse;
extern bool keyPressed[256];

// "public" functions
void printOpenGLInformation();
void initOpenGL(int *argcp, char **argv);
void start();

using Texture = GLint;

extern std::vector<RenderPass> renderPasses;
