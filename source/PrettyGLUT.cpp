#include "PrettyGLUT.hpp"

#include "Cameras.hpp"

#define switch_cam(cam)                                                        \
    do {                                                                       \
        activeCam = &cam;                                                      \
        info("Switching camera to %s\n", #cam);                                \
    } while (0)

// We need to know about this. but it's entirely game logic so it's defined in
// main.cpp.
void updateScene(double t, double dt);

// World objects
namespace PrettyGLUT {

double live_fps = 0.0;

// Display Settings
int windowWidth  = 1280;
int windowHeight = 1024;

Color colorClear = Color(0.3f, 0.2f, 0.8f);

// Input states
Vec mouse             = Vec();
int leftMouse         = 0;
GLint modifiersButton = 0;
bool keyPressed[256]  = {};

// Things to draw
std::vector<WorldObject *> drawn = std::vector<WorldObject *>();

// Cameras
ArcBallCamera arcballcam;
FreeCamera freecam;
FreeCamera fastfreecam;
FreeCamera firstPerson;

Camera *activeCam = &freecam;

void render() {
    // clear the render buffer
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    activeCam->adjustGLU();

    for (WorldObject *wo : drawn) {
        glChk();
        wo->draw();
        glChk();
    }

    // Draw each camera out of the drawn vector.
    // If we don't want it rendered, we can toggle its visibility.
    freecam.draw();
    fastfreecam.draw();
    arcballcam.draw();

    // push the back buffer to the screen
    glutSwapBuffers();
}

void resize(int w, int h) {
    windowWidth  = w;
    windowHeight = h;

    // update the viewport to fill the window
    glViewport(0, 0, w, h);

    // update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, aspectRatio(), 0.1, 100000.0);
}

void printOpenGLInformation() {
    info(
        // We don't want to break this string across lines.
        // clang-format off
         "/-----------------------------------------------------------------------\\\n"
         "| OpenGL Information                                                    |\n"
         "|-----------------------------------------------------------------------|\n"
         "|   OpenGL Version:  %50s |\n"
         "|   OpenGL Renderer: %50s |\n"
         "|   OpenGL Vendor:   %50s |\n"
        "\\-----------------------------------------------------------------------/\n",
        // clang-format on
        glGetString(GL_VERSION),
        glGetString(GL_RENDERER),
        glGetString(GL_VENDOR));
}

//
// It's all callbacks from here.
//

// The int is requied, but unused.
void doFrame(int) {
    static constexpr unsigned int delay
        = static_cast<unsigned int>(1000.0 / FPS);
    static double then            = now_secs();
    static double last_fps_update = now_secs();
    static int frames             = 0;

    // Register the next update ASAP. We want this timing to be as consistent
    // as we can get it to be.
    glutTimerFunc(delay, doFrame, 0);
    frames += 1;

    double now = now_secs();
    double dt  = now - then;
    then       = now;

    // Keep a live, running average FPS counter.
    if (now - last_fps_update > 3.0) {
        live_fps = frames / (now - last_fps_update);

        // We really only need two figures, but everyone likes seeing decimals.
        info("Average FPS: %0.1f", live_fps);

        // Reset everything.
        last_fps_update = now;
        frames          = 0;
    }
    updateScene(now, dt);

    glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
    // update the left mouse button states, if applicable
    switch (button) {
    case GLUT_LEFT_BUTTON:
        mouse           = Vec(x, y, 0.0);
        leftMouse       = state;
        modifiersButton = glutGetModifiers();
        break;
    }
}

void mouseMotion(int x, int y) {
    if (leftMouse == GLUT_DOWN) {
        const double fudge = 0.002f;

        int dx = static_cast<int>(mouse.x) - x;
        int dy = static_cast<int>(mouse.y) - y;

        mouse.x = x;
        mouse.y = y;

        // Adjust the radius of the active cam. Moves by a constant factor of
        // the idstance of the mouse moved.
        if (modifiersButton == GLUT_ACTIVE_CTRL) {
            // info("%s", dx);
            Vec dist    = Vec(dx, dy);
            auto radius = activeCam->radius();
            if (dy > 0) {
                radius = radius - fudge * dist.norm();
            } else {
                radius = radius + fudge * dist.norm();
            }
            radius = clamp(radius, 3.0, 10.0);
            activeCam->setRadius(radius);
            return;
        }

        // Adjust the rotation angles by a constant factor of the distance
        // the mouse moved.
        activeCam->rotate(fudge * dx, fudge * dy);
    }
}

void normalKeysDown(unsigned char key, int, int) {
    keyPressed[key] = true;

    switch (key) {
    case 27: // escape
        exit(0);

    case '1':
        switch_cam(freecam);
        break;

    case '2':
        switch_cam(fastfreecam);
        break;

    case '3':
        switch_cam(arcballcam);
        break;

    case '4':
        switch_cam(firstPerson);
        break;
    }
}

void normalKeysUp(unsigned char key, int, int) {
    keyPressed[key] = false;

    switch (key) {
        // Do nothing (yet?)
    }
}

// This sets up anything GLUT. This more or less never needs to change.
void initGLUT(int *argcp, char **argv) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowTitle);

    // Our boat-load of callbacks.
    glutKeyboardFunc(normalKeysDown);
    glutKeyboardUpFunc(normalKeysUp);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);

    // Misc. options
    glEnable(GL_DEPTH_TEST);
    // Since we keep track of whether keys are up or down, we don't want to spam
    // the event.
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    // Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_COLOR_MATERIAL);

    // Smooth looks so much better.
    glShadeModel(GL_SMOOTH);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void start() {
    doFrame(0);

    glutMainLoop();
}

}; // namespace PrettyGLUT
