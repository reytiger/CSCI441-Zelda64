#include "PrettyGLUT.hpp"

#include "WorldObjects.hpp"

Incallidus inc;
Firnen firnen;
FirnensCart firnenCart;
// BezierCurve halo;
Track track;
CallListObject roomFloor;
WorldSurface worldSurface;

// Defines the menu options.
// See handleRightClickMenu() and initRightClickMenu() for details.
enum MenuOpt {
    // 0 means something special to GLUT for menus, so don't pass it in.
    SwitchToFreeCam = 1,
    SwitchToFastFreeCam,
    SwitchToArcBallCam,
    SwitchToFirstCam,

    Quit,
};

// This function is expected by PrettyGLUT, because I designed it to get
// done fast, not smart. We can change this later, but this makes sure it
// builds.
// It takes in t and dt, the time and time since the last updateScene was
// called.
void updateScene(double t, double dt) {
    for (WorldObject *wo : PrettyGLUT::drawn) {
        wo->update(t, dt);
    }

    // Even though they're rendered, the cameras are NOT in the drawn list, so
    // we have to update them manually, if we want them updated at all.
    PrettyGLUT::activeCam->update(t, dt);
}

void initScene() {
    float lightCol[4]   = {1, 1, 1, 1};
    float ambientCol[4] = {0.0, 0.0, 0.0, 1.0};
    float lPosition[4] = {0.0, 0.0, 0.0, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);

    // tell OpenGL not to use the material system; just use whatever we
    // pass with glColor*()
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Cameras are important. Note, they are hard-coded in the render loop.
    // Do NOT add a camera to the  PrettyGLUT::drawn, or it'll draw (and update)
    // twice.

    // First Person!
    PrettyGLUT::firstPerson.follow(&inc);
    PrettyGLUT::firstPerson.setColor(randColor());

    // Setup controls for PrettyGLUT::freecam.
    PrettyGLUT::freecam.addWASDControls(100.0, PrettyGLUT::keyPressed);
    PrettyGLUT::freecam.moveToY(1.0);
    PrettyGLUT::freecam.setColor(randColor());

    // Cam2 is much faster.
    PrettyGLUT::fastfreecam.addWASDControls(200.0, PrettyGLUT::keyPressed);
    PrettyGLUT::fastfreecam.moveToY(1.0);
    PrettyGLUT::fastfreecam.setColor(randColor());

    // Arcballs for DAYZ.
    PrettyGLUT::arcballcam.setColor(randColor());
    PrettyGLUT::arcballcam.follow(&inc);

    // Load up Incallidus!
    PrettyGLUT::drawn.push_back(&inc);
    inc.setRadius(0.1);
    // TODO: Update to the track
    // inc.setUpdateFunc(
    //     [=](double t, double) { inc.moveTo(track.eval(0.1 * t)); });

    // Load up Firnen!
    PrettyGLUT::drawn.push_back(&firnen);
    firnen.setUpdateFunc(
        [=](double t, double dt) { firnen.updateAnimation(t, dt); });
    PrettyGLUT::drawn.push_back(&firnenCart);

    // Bezier surface!
    // PrettyGLUT::drawn.push_back(&worldSurface);

    // Track for our heros to race on!
    PrettyGLUT::drawn.push_back(&track);
    track.init();
    // track.moveTo(Vec(30.0, 30.0, 30.0));
    // track.loadFile("./assets/world/bezier-track.csv");

    PrettyGLUT::drawn.push_back(&roomFloor);
    roomFloor = CallListObject([](GLuint dl) {
        glNewList(dl, GL_COMPILE);
        auto citySize = Vec(100, 100);
        glColor3d(0.3, 0.6, 0.3);

        for (int i = -10; i <= 100 + 10; i += 1) {
            glBegin(GL_TRIANGLES);
            for (int k = -10; k <= 100 + 10; k += 1) {
                Vec off  = citySize / 100;
                auto pos = Vec(i, k) * off - citySize / 2.0;

                randColor().glSet();
                glNormal3d(0.0, 1.0, 0.0);
                glVertex3d(pos.x, 0.0, pos.y);

                glNormal3d(0.0, 1.0, 0.0);
                glVertex3d(pos.x, 0.0, pos.y - off.y);

                glNormal3d(0.0, 1.0, 0.0);
                glVertex3d(pos.x - off.x, 0.0, pos.y);

                // 2D triangle
                glNormal3d(0.0, 1.0, 0.0);
                glVertex3d(pos.x, 0.0, pos.y);

                glNormal3d(0.0, 1.0, 0.0);
                glVertex3d(pos.x, 0.0, pos.y + off.y);

                glNormal3d(0.0, 1.0, 0.0);
                glVertex3d(pos.x + off.x, 0.0, pos.y);
            }
            glEnd();
        }

        glEndList();
    });
}

void handleMainMenu(int val) {
    switch (static_cast<MenuOpt>(val)) {
    case MenuOpt::Quit:
        exit(0);
    }
}

void handleCamerasMenu(int val) {
    switch (static_cast<MenuOpt>(val)) {
    case MenuOpt::SwitchToFreeCam:
        PrettyGLUT::activeCam = &PrettyGLUT::freecam;
        break;

    case MenuOpt::SwitchToFastFreeCam:
        PrettyGLUT::activeCam = &PrettyGLUT::fastfreecam;
        break;

    case MenuOpt::SwitchToArcBallCam:
        PrettyGLUT::activeCam = &PrettyGLUT::arcballcam;
        break;

    case MenuOpt::SwitchToFirstCam:
        PrettyGLUT::activeCam = &PrettyGLUT::firstPerson;
        break;
    }
}

void initRightClickMenu() {
    int main    = glutCreateMenu(handleMainMenu);
    int cameras = glutCreateMenu(handleCamerasMenu);

    glutSetMenu(main);
    glutAddSubMenu("Switch Camera", cameras);
    glutAddMenuEntry("Quit", MenuOpt::Quit);

    glutSetMenu(cameras);
    // TODO: Highlight the current camera, somehow?
    glutAddMenuEntry("ArcBall Cam", MenuOpt::SwitchToArcBallCam);
    glutAddMenuEntry("Fast Free Cam", MenuOpt::SwitchToFastFreeCam);
    glutAddMenuEntry("First-Person Cam", MenuOpt::SwitchToFirstCam);
    glutAddMenuEntry("Free Cam", MenuOpt::SwitchToFreeCam);

    glutSetMenu(main);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
    srand(static_cast<unsigned int>(time(nullptr)));

    PrettyGLUT::initGLUT(&argc, argv);

    // It looks pretty.
    PrettyGLUT::printOpenGLInformation();

    initRightClickMenu();
    initScene();

    PrettyGLUT::start();
    return 0;
}
