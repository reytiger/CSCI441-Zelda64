#include "Controller.hpp"
#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include "fmod.hpp"

#include <fstream>

WorldObjModel level;
WorldObjModel kingRed;
Navi navi;
Md5Object *link = nullptr;

// FMOD
FMOD::System *sys = nullptr;

FMOD::Sound *hyrule_theme = nullptr;
FMOD::Sound *navi_call    = nullptr;

FMOD::Channel *themeCh = nullptr;
FMOD::Channel *callCh  = nullptr;

// Make sure navi's call comes from navi's location!
void updateNavisCallPosition() {
    auto pos         = navi.pos();
    auto vel         = navi.vel();
    FMOD_VECTOR posv = {pos.x, pos.y, pos.z};
    FMOD_VECTOR velv = {vel.x, vel.y, vel.z};

    callCh->set3DAttributes(&posv, &velv);
}

// The listener follow the active camera.
void updateListenerPosition() {
    auto pos     = activeCam->pos();
    auto vel     = activeCam->vel();
    auto forward = activeCam->lookDir().cart();
    auto up      = activeCam->up();

    FMOD_VECTOR listener_pos     = {pos.x, pos.y, pos.z};
    FMOD_VECTOR listener_vel     = {vel.x, vel.y, vel.z};
    FMOD_VECTOR listener_forward = {forward.x, forward.y, forward.z};
    FMOD_VECTOR listener_up      = {up.x, up.y, up.z};

    sys->set3DListenerAttributes(
        0, &listener_pos, &listener_vel, &listener_forward, &listener_up);

    // Move the theme music to the listener's position too!
    themeCh->set3DAttributes(&listener_pos, &listener_vel);
}

// This function is expected by PrettyGLUT, because I designed it to get
// done fast, not smart. We can change this later, but this makes sure it
// builds.
// It takes in t and dt, the time and time since the last updateScene was
// called.
void updateScene(double t, double dt) {
    // The Controller fakes keyboard input, so make sure to do it first!
    checkControllerInput(keyPressed);

    // Even though they're rendered, the cameras are NOT in the drawn list, so
    // we have to update them manually, if we want them updated at all.
    activeCam->update(t, dt);
    if (activeCam == &arcballcam) {
        link->doWASDControls(10.0f, keyPressed, true);
    } else {
        activeCam->doWASDControls(4.20f, keyPressed, true);
    }

    for (WorldObject *wo : drawn) {
        wo->update(t, dt);
    }

    // Keep FMOD's internal state up to date.
    updateListenerPosition();
    updateNavisCallPosition();

    kingRed.shader().attachUniform("time", as<float>(t));

    sys->update();

    if (keyPressed[' ']) {
        FMOD::Sound *playing = nullptr;
        callCh->getCurrentSound(&playing);
        if (playing != navi_call) {
            // The internet told me to.
            // http://stackoverflow.com/a/13838022
            sys->playSound(navi_call, nullptr, true, &callCh);
            // Make Navi much louder than the music.
            callCh->setVolume(10.0f);
            callCh->setPaused(false);

            // Lazy man's debugging.
            info("Camera position: %s\n"
                 "Camera view:     %s",
                 activeCam->pos(),
                 activeCam->lookDir());
        }
    }
}

RenderPass loadRenderPass(const std::string &name) {
    Shader vert;
    vert.loadFromFile(tfm::format("glsl/%s/vert.glsl", name), GL_VERTEX_SHADER);

    Shader frag;
    frag.loadFromFile(tfm::format("glsl/%s/frag.glsl", name),
                      GL_FRAGMENT_SHADER);

    ShaderProgram program;
    program.create();
    program.attach(vert, frag);
    program.link();

    RenderPass pass;
    pass.program(program);

    glChk();

    return pass;
}

void hideKingRed(int) {
    float theta = getRand(0, 2 * PI);
    float phi   = getRand(0, 1.0 * PI / 180);
    float r     = getRand(65, 85);
    kingRed.moveTo(VecPolar(theta, phi, r).cart());
    info("King Red has hidden!");
    glutTimerFunc(30000, hideKingRed, 0);
}

void initScene() {
    glChk();

    static Light sunlight;
    sunlight.enable();
    // Roughly the location of the sun in the skybox. The 0.0 makes it
    // directional.
    sunlight.moveTo(Vec(433.8, 975.6, -559.2, 0.0));
    drawn.push_back(&sunlight);
    glChk();

    // Loading other maps should be easy, but we've had issues.
    std::string levelPath = "assets/Env/HyruleField/hyrulefeild.obj";
    if (!level.loadObjectFile(levelPath)) {
        glChk();
        fatal("Error loading object file %s", levelPath);
    }
    glChk();

    if (!kingRed.loadObjectFile("assets/KingOfRedLions/boat.obj")) {
        fatal("Error loading object file %s", "assets/KingOfRedLions/boat.obj");
    }
    glChk();

    link = new Md5Object(
        "assets/FDL/FDL.md5mesh", "assets/FDL/FDL.md5anim", 0.1f);
    glChk();

    Shader vert;
    Shader frag;
    vert.loadFromFile("glsl/wiggly.v.glsl", GL_VERTEX_SHADER);
    frag.loadFromFile("glsl/pass_through.f.glsl", GL_FRAGMENT_SHADER);

    ShaderProgram wiggly;
    wiggly.create();
    wiggly.attach(vert, frag);
    wiggly.link();
    glChk();


    drawn.push_back(link);

    if (!navi.loadObjectFile("assets/Navi/Navi.obj")) {
        error("Unable to load Navi from .obj");
    } else {
        drawn.push_back(&navi);
        navi.follow(link);
    }

    kingRed.shader(wiggly);
    glutTimerFunc(30000, hideKingRed, 0);

    // Camera
    // Hard coded position. Just something other than a weird looking pit.
    activeCam->moveTo(Vec(19.4112, 2.85548, -13.1717));
    activeCam->lookInDir(VecPolar(-2.76918, -0.21, 1));

    arcballcam.radius(15.0);
    arcballcam.follow(link);

    // Init render passes
    renderPasses.push_back(loadRenderPass("inverted"));

    // All three of these approaches came from his blog:
    // http://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
    renderPasses.push_back(loadRenderPass("average"));
    renderPasses.push_back(loadRenderPass("lightness"));
    renderPasses.push_back(loadRenderPass("luminosity"));

    renderPasses.push_back(loadRenderPass("dot-pos"));
}

void initFMOD() {
    FMOD::System_Create(&sys);
    sys->init(100, FMOD_INIT_NORMAL, nullptr);

    FMOD_RESULT res;

    res = sys->createStream("assets/audio/HyruleField.mp3",
                            FMOD_LOOP_NORMAL | FMOD_3D,
                            nullptr,
                            &hyrule_theme);
    if (res != FMOD_OK) {
        fatal("Error opening audio file for FMOD.");
    }
    sys->playSound(hyrule_theme, nullptr, true, &themeCh);

    res = sys->createSound(
        "assets/audio/Navi,heylisten.mp3", FMOD_3D, nullptr, &navi_call);
    if (res != FMOD_OK) {
        fatal("Error opening audio file for FMOD.");
    }

    themeCh->setVolume(1.0f);
    themeCh->set3DMinMaxDistance(5.0f, 1e3f);
    themeCh->setPaused(false);

    callCh->set3DMinMaxDistance(15.0f, 1e3f);
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initFMOD();

    initOpenGL(&argc, argv);
    printOpenGLInformation();

    glutFullScreen();

    initScene();

    start();

    return 0;
}
