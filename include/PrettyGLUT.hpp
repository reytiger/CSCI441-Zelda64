#pragma once
#include "Utils.hpp"

#include "Cameras.hpp"
#include "Shader.hpp"
#include "WorldObjects.hpp"

#include "ObjectLoader.hpp"

#include <vector>

// Things to draw
// Pointers stored here can expect to live the duration of the program.
extern std::vector<WorldObject *> drawn;

// Cameras
extern FreeCamera freecam;

extern Camera *activeCam;

extern paone::Object venus;
extern paone::Object temple;

extern double live_fps;
extern double live_frametime;
extern int live_frames;

// Display settings.
// For more detailed settings, see initGLUT in PrettyGLUT.cpp.
constexpr const char *windowTitle = "Do you actually read these?";

constexpr float FOV = 60.0;

// This effectively removes any limit on rendering frames.
constexpr size_t FPS            = 9001;
constexpr auto FPS_update_delay = std::chrono::milliseconds(500);

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
void initGLUT(int *argcp, char **argv);
void start();

using Texture = GLint;

extern Texture grass;
extern Texture skybox;
extern Texture loading;
extern Texture ember;

extern ShaderProgram wigglyShader;
