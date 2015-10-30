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
