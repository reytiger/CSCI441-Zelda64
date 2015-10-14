#include "PrettyGLUT.hpp"

#include "WorldObjects.hpp"

// Incallidus inc;
// Firnen firnen;
// DragonBorn dragonBorn;
// BezierCurve halo;
FirnensCart firnenCart;
Track track;
CallListObject roomFloor;
WorldSurface worldSurface;
FlagBanner flagBanner;

// Defines the menu options.
// See handleRightClickMenu() and initRightClickMenu() for details.
enum MenuOpt {
    // 0 means something special to GLUT for menus, so don't pass it in.
    SwitchToFreeCam = 1,
    SwitchToFastFreeCam,
    SwitchToArcBallCam,
    SwitchToFirstCam,

    SwitchToIncallidus,
    SwitchToFirnen,
    SwitchToDragonBorn,

    Quit,
};


// This function is expected by PrettyGLUT, because I designed it to get
// done fast, not smart. We can change this later, but this makes sure it
// builds.
// It takes in t and dt, the time and time since the last updateScene was
// called.
void updateScene(double t, double dt) {
    for (WorldObject *wo : drawn) {
        wo->update(t, dt);
    }

    // Even though they're rendered, the cameras are NOT in the drawn list, so
    // we have to update them manually, if we want them updated at all.
    activeCam->update(t, dt);
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

    // Bezier surface!
    // TODO: draw surface as a callback
    drawn.push_back(&worldSurface);
    worldSurface.moveToY(1.0);
    worldSurface.loadControlPoints("assets/world/WorldSurfaceCPoints.csv");
    glChk();

    // Cameras are important. Note, they are hard-coded in the render loop.
    // Do NOT add a camera to the  drawn, or it'll draw (and update)
    // twice.

    // First Person!
    // FirstPerson has to be picture-in-picture viewport.
    firstPerson.follow(activeHero);
    firstPerson.setUpdateFunc([=](double /*t*/, double /*dt*/) {
        firstPerson.lookAt(dragonBorn.looking());
    });
    firstPerson.setColor(randColor());

    // Setup controls for freecam.
    freecam.addWASDControls(100.0, keyPressed);
    freecam.moveToY(1.0);
    freecam.setColor(randColor());

    // Cam2 is much faster.
    fastfreecam.addWASDControls(200.0, keyPressed);
    fastfreecam.moveToY(1.0);
    fastfreecam.setColor(randColor());

    // Arcballs for DAYZ.
    arcballcam.setColor(randColor());
    arcballcam.setRadius(5);
    arcballcam.follow(activeHero);

    // Load up Incallidus!
    drawn.push_back(&inc);
    inc.setRadius(0.2);
    inc.moveTo(worldSurface.eval(-1.2, -0.6));
    inc.setUpdateFunc([=](double /*t*/, double dt) {
        VecPolar vecTest;
        inc.lookAt(vecTest);
        // info("%s : %s", vecTest, inc.lookAt());
        // info("%s", inc.heading());
        inc.addWASDControls(100.0, keyPressed, dt, worldSurface);
        // inc.moveTo(worldSurface.eval(inc.pos().x, inc.pos().z));
    });

    // Load up Firnen!
    drawn.push_back(&firnen);
    // TODO: move cart to firnen class
    // drawn.push_back(&firnenCart);
    firnen.load();
    firnen.setUpdateFunc([=](double t, double /*dt*/) {
        auto arc = 10.0 * t;
        firnen.moveTo(track.eval_arc(arc));
        firnen.lookAt(track.eval_deriv_arc(arc));
    });

    // Load up our DragonBorn!
    drawn.push_back(&dragonBorn);
    dragonBorn.setUpdateFunc([=](double t, double /*dt*/) {
        auto param = 0.03 * t;
        dragonBorn.moveTo(track.eval_t(param));
        dragonBorn.lookAt(track.eval_deriv_t(param));
    });


    // Objects on the world surface.
    drawn.push_back(&flagBanner);
    flagBanner.init();
    flagBanner.setUpdateFunc(
        [=](double t, double dt) { flagBanner.updateAnimation(t, dt); });

    // Track for our heros to race on!
    drawn.push_back(&track);
    track.init();
    // track.moveTo(Vec(30.0, 30.0, 30.0));
    // track.loadFile("./assets/world/bezier-track.csv");

    // drawn.push_back(&roomFloor);
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

                // 2nd triangle
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

    default:
        info("Unhandled menu case: %d", val);
    }
}

void handleCamerasMenu(int val) {
    switch (static_cast<MenuOpt>(val)) {
    case MenuOpt::SwitchToFreeCam:
        switch_cam(freecam);
        break;
    case MenuOpt::SwitchToFastFreeCam:
        switch_cam(fastfreecam);
        break;
    case MenuOpt::SwitchToArcBallCam:
        switch_cam(arcballcam);
        break;
    case MenuOpt::SwitchToFirstCam:
        switch_cam(firstPerson);
        break;

    default:
        info("Unhandled menu case: %d", val);
    }
}

void handleHerossMenu(int val) {
    switch (static_cast<MenuOpt>(val)) {
    case MenuOpt::SwitchToIncallidus:
        switch_hero(inc);
        break;
    case MenuOpt::SwitchToFirnen:
        switch_hero(firnen);
        break;
    case MenuOpt::SwitchToDragonBorn:
        switch_hero(dragonBorn);
        break;

    default:
        info("Unhandled menu case: %d", val);
    }
    arcballcam.follow(activeHero);
    firstPerson.follow(activeHero);
}

void initRightClickMenu() {
    int main    = glutCreateMenu(handleMainMenu);
    int cameras = glutCreateMenu(handleCamerasMenu);
    int heros   = glutCreateMenu(handleHerossMenu);

    glutSetMenu(main);
    glutAddSubMenu("Switch Camera", cameras);
    glutAddSubMenu("Switch Hero", heros);
    glutAddMenuEntry("Quit", MenuOpt::Quit);

    glutSetMenu(cameras);
    // TODO: Highlight the current camera, somehow?
    glutAddMenuEntry("ArcBall Cam", MenuOpt::SwitchToArcBallCam);
    glutAddMenuEntry("Fast Free Cam", MenuOpt::SwitchToFastFreeCam);
    glutAddMenuEntry("First-Person Cam", MenuOpt::SwitchToFirstCam);
    glutAddMenuEntry("Free Cam", MenuOpt::SwitchToFreeCam);

    glutSetMenu(heros);
    glutAddMenuEntry("Follow Incallidus", MenuOpt::SwitchToIncallidus);
    glutAddMenuEntry("Follow Firnen", MenuOpt::SwitchToFirnen);
    glutAddMenuEntry("Follow Dragon Born", MenuOpt::SwitchToDragonBorn);

    glutSetMenu(main);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
    srand(static_cast<unsigned int>(time(nullptr)));

    initGLUT(&argc, argv);

    // It looks pretty.
    printOpenGLInformation();

    initRightClickMenu();
    initScene();

    startGuildWars();
    return 0;
}
