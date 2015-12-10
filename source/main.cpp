#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include <fstream>

paone::Object levelBongo;
paone::Object levelHyruleField;

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
    // if (!levelBongo.loadObjectFile("assets/Env/HyruleField/hyrulefeild.obj"))
    // {
    //     abort();
    // }

    if (!levelHyruleField.loadObjectFile(
            "assets/Env/HyruleField/hyrulefeild.obj")) {
        abort();
    }
    glChk();

    // Camera
    activeCam->radius(150.0);

    // All three of these approaches came from his blog:
    // http://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
    // renderPasses.push_back(loadRenderPass("average"));
    // renderPasses.push_back(loadRenderPass("lightness"));
    renderPasses.push_back(loadRenderPass("luminosity"));
}

void initTextures() {
    grass = SOIL_load_OGL_texture("assets/textures/minecraft.jpg",
                                  SOIL_LOAD_AUTO,
                                  SOIL_CREATE_NEW_ID,
                                  SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
                                      | SOIL_FLAG_NTSC_SAFE_RGB
                                      | SOIL_FLAG_COMPRESS_TO_DXT);
    glChk();
    {
        glBindTexture(GL_TEXTURE_2D, grass);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glChk();

    skybox = SOIL_load_OGL_texture("assets/textures/clouds-skybox.jpg",
                                   SOIL_LOAD_AUTO,
                                   SOIL_CREATE_NEW_ID,
                                   SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
                                       | SOIL_FLAG_NTSC_SAFE_RGB
                                       | SOIL_FLAG_COMPRESS_TO_DXT);
    glChk();
    {
        glBindTexture(GL_TEXTURE_2D, skybox);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    glChk();
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initOpenGL(&argc, argv);
    printOpenGLInformation();

    initTextures();
    initScene();

    start();

    return 0;
}
