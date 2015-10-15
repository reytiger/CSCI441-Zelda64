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
extern FreeCamera fastfreecam; // TODO: Make this a first person.

// Heros
extern Incallidus inc;
extern Firnen firnen;
extern DragonBorn dragonBorn;

extern Camera *activeCam;
extern WorldObject *activeHero;

extern double live_fps;

// Display settings.
// For more detailed settings, see initGLUT in PrettyGLUT.cpp.
constexpr const char *windowTitle = "Skyrim belongs to the Nords!";

constexpr double FPS = 60.0;
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
void startGuildWars();
