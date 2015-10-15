#include "PrettyGLUT.hpp"

#include "fmod.hpp"

#include "WorldObjects.hpp"
#include "yaml-cpp/yaml.h"

#include <array>

FirnensCart firnenCart;
Track track;
CallListObject roomFloor;
WorldSurface worldSurface;
FlagBanner flagBanner;

Spotlight spotlight;
std::array<PointLight, 3> pointLights;

// FMOD
FMOD::System *sys = nullptr;

FMOD::Sound *skyrim_theme = nullptr;
FMOD::Sound *skyrim_shout = nullptr;

FMOD::Channel *themeCh = nullptr;
FMOD::Channel *shoutCh = nullptr;

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

void loadFromFile(std::string file) {

    // lets first read in the yaml file
    YAML::Node node = YAML::LoadFile(file);

    // ok, now lets get all the world Surface infomation and save it
    YAML::Node worldCPoints = node["WorldCPoints"];
    YAML::Node w_points = worldCPoints["points"];
    if (w_points.size() != worldCPoints["numberOfPoints"].as<size_t>()) {
        fatal("The number of points spesified for the world does not match");
    }
    std::vector<Vec> worldVecPoints;
    for (std::size_t i = 0; i < w_points.size(); i++) {
        double x = w_points[i][0].as<double>();
        double y = w_points[i][1].as<double>();
        double z = w_points[i][2].as<double>();
        Vec v = Vec(x, y, z);
        worldVecPoints.push_back(v);
    }
    worldSurface.setControlPoints(worldVecPoints);
    worldSurface.setZmaxmin();

    // sweet, time to get the trees
    // YAML::Node
}

// TODO: Make these classes
void updateSource() {
    // Place the theme on the active hero
    auto pos             = activeHero->pos();
    FMOD_VECTOR themePos = {pos.x, pos.y, pos.z};

    auto vel             = activeHero->vel();
    FMOD_VECTOR themeVel = {vel.x, vel.y, vel.z};

    themeCh->set3DAttributes(&themePos, &themeVel);

    // And the shout on Dragonborn!
    pos                       = dragonBorn.pos();
    FMOD_VECTOR dragonBornPos = {pos.x, pos.y, pos.z};

    vel                       = dragonBorn.vel();
    FMOD_VECTOR dragonbornVel = {vel.x, vel.y, vel.z};

    shoutCh->set3DAttributes(&dragonBornPos, &themeVel);
}

void updateListener() {
    auto pos                 = activeCam->pos();
    FMOD_VECTOR listener_pos = {pos.x, pos.y, pos.z};

    auto vel                 = activeCam->vel();
    FMOD_VECTOR listener_vel = {vel.x, vel.y, vel.z};

    auto forward                 = activeCam->lookAt();
    FMOD_VECTOR listener_forward = {forward.x, forward.y, forward.z};

    auto up                 = activeCam->up();
    FMOD_VECTOR listener_up = {up.x, up.y, up.z};

    sys->set3DListenerAttributes(
        0, &listener_pos, &listener_vel, &listener_forward, &listener_up);
}

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

    // The internet tutorials said we should.
    sys->update();

    // Demonstrate your might with the press of the space bar.
    // TODO: Make this positional.
    if (keyPressed[' ']) {
        FMOD::Sound *playing = nullptr;
        shoutCh->getCurrentSound(&playing);
        if (playing != skyrim_shout) {
            // The internet told me to.
            // http://stackoverflow.com/a/13838022
            sys->playSound(skyrim_shout, nullptr, true, &shoutCh);
            // Make the shout much louder than the music.
            shoutCh->setVolume(15.0f);
            shoutCh->setPaused(false);
        }
    }

    // Move the source with the active Hero.
    updateSource();

    // And the listener with the active camera.
    updateListener();
}

void initScene() {
    for (PointLight &light : pointLights) {
        drawn.push_back(&light);

        light.enable();
        light.diffuse((0.15 * randColor()).v);
        light.specular((0.15 * randColor()).v);

        double a = 3.0 * getRandd() - 1.0;
        double b = 3.0 * getRandd() - 1.0;
        double c = 5.0 * getRandd() + 0.5;

        light.setUpdateFunc([&light, a, b, c](double t, double /*dt*/) {
            auto phi = M_PI / 2.0 * (0.5 * sin(b * t) + 0.5);
            auto vp  = VecPolar(a * t, phi, c);
            auto pos = vp.cart() + Vec(0.0, 5.0, 0.0);
            light.moveTo(pos);
        });
    }

    // Make them follow people!
    pointLights[0].follow(&inc);
    pointLights[1].follow(&firnen);
    pointLights[2].follow(&dragonBorn);

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

        auto vp  = VecPolar(0.68 * t, 0.0, 5.0);
        auto pos = vp.cart() + Vec(0.0, 5.0, 0.0);
        spotlight.moveTo(pos);
        spotlight.lookAt((Vec() - spotlight.pos()));
    });

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
    arcballcam.follow(&inc);

    // Load up Incallidus!
    drawn.push_back(&inc);
    inc.setRadius(0.2);
    inc.moveTo(worldSurface.eval(-1.2, -0.6));
    inc.setUpdateFunc([=](double t, double dt) {
        VecPolar vecTest;
        inc.lookAt(-inc.pos());
        inc.setRadius(0.1 * cos(t) + 0.5);
        if (activeHero == &inc
            && (activeCam == &arcballcam || activeCam == &firstPerson)) {
            inc.addWASDControls(100.0, keyPressed, dt, worldSurface);
        }
    });

    // Load up Firnen!
    drawn.push_back(&firnen);
    firnen.load();
    firnen.setUpdateFunc([=](double t, double /*dt*/) {
        auto arc = 10.0 * t;
        firnen.moveTo(track.eval_arc(arc));
        firnen.lookAt(track.eval_deriv_arc(arc));
    });

    // Load up our DragonBorn!
    drawn.push_back(&dragonBorn);
    dragonBorn.setUpdateFunc([=](double t, double /*dt*/) {
        auto param = 0.01 * t;
        dragonBorn.moveTo(track.eval_t(param));
        dragonBorn.lookAt(track.eval_deriv_t(param));
        dragonBorn.setRadius(0.5 * cos(t) + 1.5);
    });

    // Objects on the world surface.
    drawn.push_back(&flagBanner);
    flagBanner.init();
    flagBanner.moveTo(Vec(-2.0, 0.3, 2.0));
    flagBanner.setUpdateFunc(
        [=](double t, double dt) { flagBanner.updateAnimation(t, dt); });

    // Track for our heros to race on!
    drawn.push_back(&track);
    track.init();
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

// This blog post was helpful.
// https://katyscode.wordpress.com/2012/10/05/cutting-your-teeth-on-fmod-part-1-build-environment-initialization-and-playing-sounds/
// So was this one:
// http://glasnost.itcarlow.ie/~powerk/technology/FMOD/FMOD%203d%20Settings.html
// Initalizes FMOD.
void initFMOD() {
    FMOD::System_Create(&sys);
    sys->init(100, FMOD_INIT_NORMAL, nullptr);

    // In their tongue, he is Dovahkiin... DRAGONBORN!
    sys->createStream("assets/audio/skyrim-theme.mp3",
                      FMOD_LOOP_NORMAL | FMOD_3D,
                      nullptr,
                      &skyrim_theme);
    sys->playSound(skyrim_theme, nullptr, true, &themeCh);

    // Do not mess with Dovahkiin.
    sys->createSound(
        "assets/audio/skyrim-shout.mp3", FMOD_3D, nullptr, &skyrim_shout);

    themeCh->setVolume(1.0f);
    themeCh->set3DMinMaxDistance(5.0f, 1e3f);
    themeCh->setPaused(false);

    shoutCh->set3DMinMaxDistance(15.0f, 1e3f);
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    loadFromFile("assets/world/Windhelm.yaml");

    initFMOD();
    initGLUT(&argc, argv);

    // It looks pretty.
    printOpenGLInformation();

    initRightClickMenu();
    initScene();

    startGuildWars();
    return 0;
}
