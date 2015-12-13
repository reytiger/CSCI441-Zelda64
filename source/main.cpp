#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include <fstream>

paone::Object levelBongo;
paone::Object levelHyruleField;
Navi navi;
Md5Object *link = nullptr;

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

    // if (!levelBongo.loadObjectFile("assets/Env/HyruleField/hyrulefeild.obj"))
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

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initOpenGL(&argc, argv);
    printOpenGLInformation();

    initScene();

    start();

    return 0;
}
