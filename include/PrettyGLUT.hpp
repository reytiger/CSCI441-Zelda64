#pragma once
#include "Utils.hpp"

#include "Cameras.hpp"
#include "WorldObjects.hpp"

#include <vector>

#define switch_cam(cam)                                                        \
    do {                                                                       \
        activeCam = &cam;                                                      \
        info("Switching camera to %s\n", #cam);                                \
    } while (0)

#define switch_hero(hero)                                                      \
    do {                                                                       \
        activeHero = &hero;                                                    \
        info("Following %s\n", #hero);                                         \
    } while (0)

// Things to draw
// Pointers stored here can expect to live the duration of the program.
extern std::vector<WorldObject *> drawn;

// Cameras
extern ArcBallCamera arcballcam;
extern FreeCamera firstPerson;
extern FreeCamera freecam;
extern FreeCamera fastfreecam;
extern FreeCamera backcam;

// Heros
extern Incallidus inc;

extern Camera *activeCam;
extern WorldObject *activeHero;

extern float live_fps;
extern int live_frames;

// Display settings.
// For more detailed settings, see initGLUT in PrettyGLUT.cpp.
constexpr const char *windowTitle = "Do you actually read these?";

// This effectively removes any limit on rendering frames.
constexpr float FPS              = 9001.0;
constexpr float FOV              = 60.0;
constexpr float FPS_update_delay = 0.5;

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
