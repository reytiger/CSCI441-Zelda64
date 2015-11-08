#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include <array>

CallListObject roomFloor;
paone::Object model;
paone::Object model2;
PointLight light;

Incallidus inc;

Texture pattern;
Texture skybox;

FountainSystem fountain;

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
    // activeCam->doWASDControls(25.0, keyPressed, true);

    shaderDemo.attachUniform("time", t);

    inc.moveTo(clamp(inc.pos(), Vec(-100, 0, -100), Vec(100, 0, 100)));
}

void initScene() {
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Color(1.0, 1.0, 1.0).v);

    drawn.push_back(&light);
    light.enable();

    light.moveToY(50.0);

    light.setUpdateFunc([&](double t, double /*dt*/) {
        t /= 5.0;
        auto color
            = 0.3 * Color(cos(3.0 * t), cos(5.0 * t), cos(1.0 * t)) + 0.6;
        light.diffuse(color.v);
        light.specular(color.v);
    });

    // Floor
    drawn.push_back(&roomFloor);
    roomFloor = CallListObject([&](GLuint dl) {
        glNewList(dl, GL_COMPILE);

        Material().set();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, pattern.handle);

        static const auto halfsize = Vec(100, 100);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 1.0f, 0.0f);

        glTexCoord2f(-halfsize.x, halfsize.y);
        glVertex3d(-halfsize.x, 0, halfsize.y);

        glTexCoord2f(halfsize.x, halfsize.y);
        glVertex3d(halfsize.x, 0, halfsize.y);

        glTexCoord2f(halfsize.x, -halfsize.y);
        glVertex3d(halfsize.x, 0, -halfsize.y);

        glTexCoord2f(-halfsize.x, -halfsize.y);
        glVertex3d(-halfsize.x, 0, -halfsize.y);

        glEnd();

        glDisable(GL_TEXTURE_2D);
        glEndList();
    });
    glChk();

    drawn.push_back(&fountain);
    fountain.material(Material::Brass);
    fountain.moveTo(0, 10, 0);
    fountain.radius(5.0f);

    drawn.push_back(&inc);
    inc.setUpdateFunc([&](double /*t*/, double /*dt*/) {
        inc.doWASDControls(15.0, keyPressed, false);
    });
    inc.moveToY(roomFloor.pos().y + 0.1);

    activeCam->follow(&inc);
    activeCam->radius(50.0);

    auto pt = model.getLocation();
    assert(pt);
    pt->setY(4.7);
    model.loadObjectFile("assets/venus.obj");

    pt = model2.getLocation();
    assert(pt);
    pt->setX(3);
    pt->setY(4.7);
    model2.loadObjectFile("assets/venus.obj");
}

void initTextures() {
    pattern.handle = SOIL_load_OGL_texture(
        "assets/textures/minecraft.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
            | SOIL_FLAG_COMPRESS_TO_DXT);
    glChk();
    {
        glBindTexture(GL_TEXTURE_2D, pattern.handle);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glChk();

    skybox.handle = SOIL_load_OGL_texture("assets/textures/clouds-skybox.jpg",
                                          SOIL_LOAD_AUTO,
                                          SOIL_CREATE_NEW_ID,
                                          SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
                                              | SOIL_FLAG_NTSC_SAFE_RGB
                                              | SOIL_FLAG_COMPRESS_TO_DXT);
    glChk();
    {
        glBindTexture(GL_TEXTURE_2D, skybox.handle);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    glChk();
}

void initShaders(const std::string &vertFilename,
                 const std::string &fragFilename) {
    Shader vert;
    vert.loadFromFile(vertFilename, GL_VERTEX_SHADER);

    Shader frag;
    frag.loadFromFile(fragFilename, GL_FRAGMENT_SHADER);

    shaderDemo.create();
    shaderDemo.link(vert, frag);

    glChk();
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initGLUT(&argc, argv);
    glewInit();
    printOpenGLInformation();

    // Handle shader filenames.
    std::string file1 = argc >= 2 ? argv[1] : "glsl/pass_through.v.glsl";
    std::string file2 = argc >= 3 ? argv[2] : "glsl/pass_through.f.glsl";

    if (argc < 2) {
        warn("Expected filenames, defaulting to '%s' and '%s'.", file1, file2);
    }
    initShaders(file1, file2);

    initTextures();
    initScene();

    start();

    return 0;
}
