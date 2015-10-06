#include "PrettyGLUT.hpp"

#include "WorldObjects.hpp"

Incallidus inc;
BezierCurve halo;
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
    PrettyGLUT::arcballcam.follow(&PrettyGLUT::freecam);
    // PrettyGLUT::arcballcam.addArcBallControls(???);

    // Load up Incallidus!
    PrettyGLUT::drawn.push_back(&inc);
    inc.setRadius(0.1);

    PrettyGLUT::drawn.push_back(&worldSurface);

    PrettyGLUT::drawn.push_back(&halo);
    halo.loadFile("./assets/world/bezier-halo.csv");
    halo.moveToZ(5.0);
    halo.setUpdateFunc([](double t, double dt) {
        auto theta = t;
        halo.moveTo(Vec(cos(1.1 * theta),
                        sin(1.3 * theta),
                        cos(1.5 * theta) * sin(0.9 * theta)));
    });

    PrettyGLUT::drawn.push_back(&roomFloor);
    roomFloor = CallListObject([](GLuint dl) {
        glNewList(dl, GL_COMPILE);
        auto citySize = Vec(100, 100);
        glColor3d(0.3, 0.6, 0.3);

        for (int i = -10; i <= 100 + 10; i += 1) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int k = -10; k <= 100 + 10; k += 1) {
                Vec off  = citySize / 100;
                auto pos = Vec(i, k) * off - citySize / 2.0;

                // glColor3d(getRand(), getRand(), getRand());
                glVertex3d(pos.x, 0.0, pos.y);
                glNormal3d(0.0, 1.0, 0.0);

                glVertex3d(pos.x, 0.0, pos.y - off.y);
                glNormal3d(0.0, 1.0, 0.0);

                glVertex3d(pos.x - off.x, 0.0, pos.y);
                glNormal3d(0.0, 1.0, 0.0);


                // glColor3d(getRand(), getRand(), getRand());
                glVertex3d(pos.x, 0.0, pos.y);
                glNormal3d(0.0, 1.0, 0.0);

                glVertex3d(pos.x, 0.0, pos.y + off.y);
                glNormal3d(0.0, 1.0, 0.0);

                glVertex3d(pos.x + off.x, 0.0, pos.y);
                glNormal3d(0.0, 1.0, 0.0);
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
        info("First person camera is not implemented. Sorry.");
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
    srand(time(nullptr));

    PrettyGLUT::initGLUT(&argc, argv);

    // It looks pretty.
    PrettyGLUT::printOpenGLInformation();

    initRightClickMenu();
    initScene();

    PrettyGLUT::start();
    return 0;
}
