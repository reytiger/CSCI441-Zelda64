#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include <array>

CallListObject roomFloor;

Spotlight spotlight;
std::array<PointLight, 3> pointLights;

// Defines the menu options.
// See handleRightClickMenu() and initRightClickMenu() for details.
enum MenuOpt {
    // 0 means something special to GLUT for menus, so don't pass it in.
    SwitchToFreeCam = 1,
    SwitchToFastFreeCam,
    SwitchToArcBallCam,
    SwitchToFirstCam,
    SwitchToBackCam,

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

    if (activeCam == &fastfreecam) {
        activeCam->doWASDControls(20.0, keyPressed, true);
    } else if (activeCam == &freecam) {
        activeCam->doWASDControls(10.0, keyPressed, true);
    } else {
        activeHero->doWASDControls(5.0, keyPressed);
    }
}

void initScene() {

    // Lights
    for (PointLight &light : pointLights) {
        drawn.push_back(&light);

        light.enable();
        light.diffuse((0.15f * randColor()).v);
        light.specular((0.15f * randColor()).v);

        float a = 3.0f * getRand() - 1.0f;
        float b = 3.0f * getRand() - 1.0f;
        float c = 5.0f * getRand() + 0.5f;

        light.setUpdateFunc([&light, a, b, c](double t, double /*dt*/) {
            auto phi = PI / 2.0 * (0.5 * sin(b * t) + 0.5);
            auto vp  = VecPolar(a * t, phi, c);
            auto pos = vp.cart() + Vec(0, 5, 0);
            light.moveTo(pos);
        });
    }

    // Make them follow people!
    pointLights[0].follow(&inc);

    drawn.push_back(&spotlight);
    spotlight.enable();
    spotlight.diffuse(Color(0.2, 0.2, 0.2).v);
    spotlight.exponent(1.0);
    spotlight.cutoff(12.0);

    // Spin in a circle at Y=10.0.
    spotlight.setUpdateFunc([&](double t, double /*dt*/) {
        auto color
            = 0.05 * Color(cos(3.0 * t), cos(5.0 * t), cos(1.0 * t)) + 0.05;
        spotlight.ambient(color.v);
        spotlight.diffuse(color.v);
        spotlight.specular(color.v);

        auto vp  = VecPolar(0.68f * t, 0.0f, 5.0f);
        auto pos = vp.cart() + Vec(0.0, 5.0, 0.0);
        spotlight.moveTo(pos);
        spotlight.lookInDir((Vec() - spotlight.pos()));
    });

    // Floor
    drawn.push_back(&roomFloor);
    roomFloor = CallListObject([&](GLuint dl) {
        glNewList(dl, GL_COMPILE);

        static const auto size = Vec(100, 100);

        for (int i = -1; i <= size.x + 1; i += 1) {
            glBegin(GL_TRIANGLE_STRIP);
            // The terrain is flat. All of the normals are straight up.
            glNormal3f(0.0, 1.0, 0.0);
            for (int k = -1; k <= size.y; k += 1) {
                Vec off  = Vec(2.0, 2.0);
                auto pos = Vec(i, k) * off - Vec(200.0, 200.0) / 2.0;

                Material::GreenRubber.set();

                glVertex3d(pos.x, 0.0, pos.y);
                glVertex3d(pos.x, 0.0, pos.y - off.y);
                glVertex3d(pos.x - off.x, 0.0, pos.y);

                // Skip the second triangle on extreme cases.
                // TODO: This is probably wrong.
                if (k == size.y || i == size.x) {
                    break;
                }

                glVertex3d(pos.x, 0.0, pos.y);
                glVertex3d(pos.x, 0.0, pos.y + off.y);
                glVertex3d(pos.x + off.x, 0.0, pos.y);
            }
            glEnd();
        }

        glEndList();
    });


    // Cameras!

    // First Person!
    // FirstPerson has to be picture-in-picture viewport.
    firstPerson.follow(activeHero);
    firstPerson.setUpdateFunc([=](double /*t*/, double /*dt*/) {
        firstPerson.lookInDir(activeHero->lookDir());
    });

    backcam.follow(activeHero);
    backcam.setUpdateFunc([=](double /*t*/, double /*dt*/) {
        Vec forward = activeHero->lookDir().cart();
        backcam.lookInDir(-forward);
    });

    // Setup controls for freecam.
    freecam.moveTo(Vec(20.0, 20.0, 20.0));
    freecam.lookAtThing(Vec());

    // Cam2 is much faster.
    fastfreecam.moveTo(Vec(20.0, 20.0, 20.0));
    fastfreecam.lookAtThing(Vec());

    // Arcballs for DAYZ.
    arcballcam.radius(5);
    arcballcam.follow(&inc);

    // Action? (Our Hero!)

    // Load up Incallidus!
    drawn.push_back(&inc);
    inc.radius(0.2f);
    inc.setUpdateFunc([=](double t, double /*dt*/) {
        inc.radius(0.1f * as<float>(cos(t)) + 0.5f);
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
    case MenuOpt::SwitchToBackCam:
        switch_cam(backcam);
        break;

    default:
        info("Unhandled menu case: %d", val);
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
    glutAddMenuEntry("Rev First-Person Cam", MenuOpt::SwitchToBackCam);
    glutAddMenuEntry("Free Cam", MenuOpt::SwitchToFreeCam);

    glutSetMenu(main);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initGLUT(&argc, argv);

    // It looks pretty.
    printOpenGLInformation();

    initRightClickMenu();
    initScene();

    start();

    return 0;
}
