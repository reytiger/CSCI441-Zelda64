#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include "fmod.hpp"

#include <fstream>

paone::Object levelBongo;
paone::Object levelHyruleField;
Navi navi;
Md5Object *link = nullptr;

// FMOD
FMOD::System *sys = nullptr;

FMOD::Sound *hyrule_theme = nullptr;
FMOD::Sound *navi_call    = nullptr;

FMOD::Channel *themeCh = nullptr;
FMOD::Channel *callCh  = nullptr;

// This function is expected by PrettyGLUT, because I designed it to get
// done fast, not smart. We can change this later, but this makes sure it
// builds.
// It takes in t and dt, the time and time since the last updateScene was
// called.
void updateScene(double t, double dt) {
    // Even though they're rendered, the cameras are NOT in the drawn list, so
    // we have to update them manually, if we want them updated at all.
    activeCam->update(t, dt);
    activeCam->doWASDControls(2.0, keyPressed, true);

    for (WorldObject *wo : drawn) {
        wo->update(t, dt);
    }

    // TODO: Make this positional.
    if (keyPressed[' ']) {
        FMOD::Sound *playing = nullptr;
        callCh->getCurrentSound(&playing);
        if (playing != navi_call) {
            // The internet told me to.
            // http://stackoverflow.com/a/13838022
            sys->playSound(navi_call, nullptr, true, &callCh);
            // Make Navi much louder than the music.
            callCh->setVolume(5.0f);
            callCh->setPaused(false);
        }
    }
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

void initScene() {
    glChk();

    // if
    // (!levelBongo.loadObjectFile("assets/Env/HyruleField/hyrulefeild.obj"))
    // {
    //     fatal("Error loading object file %s",
    //     "assets/Env/HyruleField/hyrulefeild.obj");
    // }
    // glChk();

    if (!levelHyruleField.loadObjectFile(
            "assets/Env/HyruleField/hyrulefeild.obj")) {
        fatal("Error loading object file %s",
              "assets/Env/HyruleField/hyrulefeild.obj");
    }
    glChk();

    link = new Md5Object(
        "assets/FDL/FDL.md5mesh", "assets/FDL/FDL.md5anim", 0.1f);
    glChk();
    drawn.push_back(link);

    if (!navi.loadObjectFile("assets/Navi/Navi.obj")) {
        error("Unable to load Navi from .obj");
    } else {
        drawn.push_back(&navi);
        navi.follow(link);
    }

    // Camera
    // TODO: Why is this here?
    activeCam->radius(150.0);

    // Init render passes

    // All three of these approaches came from his blog:
    // http://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
    // renderPasses.push_back(loadRenderPass("average"));
    // renderPasses.push_back(loadRenderPass("lightness"));
    // renderPasses.push_back(loadRenderPass("luminosity"));

    // renderPasses.push_back(loadRenderPass("inverted"));
}

void initFMOD() {
    FMOD::System_Create(&sys);
    sys->init(100, FMOD_INIT_NORMAL, nullptr);

    sys->createStream("assets/audio/HyruleField.mp3",
                      FMOD_LOOP_NORMAL | FMOD_3D,
                      nullptr,
                      &hyrule_theme);
    sys->playSound(hyrule_theme, nullptr, true, &themeCh);

    sys->createSound(
        "assets/audio/Navi,heylisten.mp3", FMOD_3D, nullptr, &navi_call);

    themeCh->setVolume(1.0f);
    themeCh->set3DMinMaxDistance(5.0f, 1e3f);
    themeCh->setPaused(false);

    callCh->set3DMinMaxDistance(15.0f, 1e3f);
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initOpenGL(&argc, argv);
    printOpenGLInformation();

    initFMOD();

    initScene();

    start();

    return 0;
}
