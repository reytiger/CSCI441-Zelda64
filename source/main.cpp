#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include <array>

CallListObject roomFloor;
CallListObject vulcano;

paone::Object venus;
paone::Object temple;
PointLight light;

Incallidus inc;

Texture grass;
Texture skybox;
Texture ember;

FountainSystem vulSpout;
FountainSystem incSpell;

bool castingSpell = false;

float vulHeight     = 50.0;
float vulBaseRadius = 30.0;

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

void startCasting() {
    castingSpell = true;
    drawn.push_back(&incSpell);
}

// The int is requried, but unused.
void stopCasting(int) {
    castingSpell = false;
    auto pos = std::find(drawn.begin(), drawn.end(), &incSpell);
    if (pos != drawn.end()) {
        drawn.erase(pos);
    }
    incSpell.clear();
}

void initSpell(FountainSystem &spell) {
    spell.radius(0.05);

    spell.min_cone_theta = -10.0f; // degrees
    spell.max_cone_theta = 10.0f;  // degrees

    spell.min_cone_phi = -10; // degrees
    spell.max_cone_phi = 10;  // degrees

    spell.min_speed = 10.0f; // meters
    spell.max_speed = 20.0f; // meters

    spell.min_life = 0.5f; // seconds
    spell.max_life = 1.0f; // seconds

    spell.spawn_rate = 5000.0f; // particles per second

    spell.gravity = false;
}

// This function is expected by PrettyGLUT, because I designed it to get
// done fast, not smart. We can change this later, but this makes sure it
// builds.
// It takes in t and dt, the time and time since the last updateScene was
// called.
void updateScene(double t, double dt) {
    // Even though they're rendered, the cameras are NOT in the drawn list, so
    // we have to update them manually, if we want them updated at all.
    activeCam->update(t, dt);
    // activeCam->doWASDControls(25.0, keyPressed, true);

    // Tell the fountains where to 'look' - same as the camera.
    vulSpout.lookInDir(activeCam->lookDir());

    wigglyShader.attachUniform("time", 1000.0 + t);

    inc.moveTo(clamp(inc.pos(), Vec(-100, 0.5, -100), Vec(100, 0.5, 100)));

    if (keyPressed[' '] && !castingSpell) {
        startCasting();
        glutTimerFunc(5000, stopCasting, 0);
    }

    for (WorldObject *wo : drawn) {
        wo->update(t, dt);
    }
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
        glEnable(GL_CULL_FACE);
        glBindTexture(GL_TEXTURE_2D, grass);

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

    // Vulcano
    drawn.push_back(&vulcano);
    vulcano.moveTo(-50, 0, -50);
    vulcano.material(Material::Obsidian);

    static auto vulcano_body = gluNewQuadric();
    static auto vulcano_top  = gluNewQuadric();

    vulcano = CallListObject([&](GLuint dl) {
        glNewList(dl, GL_COMPILE);
        glDisable(GL_CULL_FACE);

        wigglyShader.use();

        pushMatrixAnd([&]() {
            glRotatef(-90.0f, 1, 0, 0);
            gluCylinder(vulcano_body,
                        vulBaseRadius,
                        vulBaseRadius / 4.0,
                        vulHeight,
                        20,
                        20);
        });

        pushMatrixAnd([&]() {
            glTranslatef(0.0f, vulHeight - 0.25f, 0.0f);
            glRotatef(90.0f, -1, 0, 0);
            gluDisk(vulcano_top, 0, vulBaseRadius / 4.0, 20, 1);
        });

        glEndList();
    });

    // Vulcano Spout
    drawn.push_back(&vulSpout);
    vulSpout.moveTo(vulcano.pos());
    vulSpout.moveToY(vulHeight - 1.0);

    vulSpout.material(Material::Brass);
    vulSpout.radius(0.5);

    vulSpout.min_speed = 20.0f;                    // meters
    vulSpout.max_speed = 2.0 * vulSpout.min_speed; // meters

    vulSpout.min_life = 7.0f; // seconds
    vulSpout.max_life = 8.0f; // seconds

    vulSpout.spawn_rate = 750.0f; // particles per second.

    vulSpout.tex(ember);

    // Our Hero!
    drawn.push_back(&inc);
    inc.setUpdateFunc([&](double /*t*/, double /*dt*/) {
        inc.doWASDControls(25.0, keyPressed, false);
    });

    // His spell
    incSpell.follow(&inc);
    initSpell(incSpell);

    // Camera
    activeCam->follow(&inc);
    activeCam->radius(150.0);

    // Venus 1
    auto pt = venus.getLocation();
    assert(pt);
    pt->setY(8.7);
    pt->setX(-50);
    pt->setZ(50);
    venus.loadObjectFile("assets/venus.obj");

    // Venus 2
    pt = temple.getLocation();
    assert(pt);
    temple.loadObjectFile("assets/temple.obj");
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

    ember = SOIL_load_OGL_texture("assets/textures/ember.png",
                                  SOIL_LOAD_AUTO,
                                  SOIL_CREATE_NEW_ID,
                                  SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
                                      | SOIL_FLAG_NTSC_SAFE_RGB
                                      | SOIL_FLAG_COMPRESS_TO_DXT);
    glChk();
    {
        glBindTexture(GL_TEXTURE_2D, ember);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glChk();
}

void initShaders() {

    // Venus
    {
        Shader vert;
        vert.loadFromFile("glsl/wiggly.v.glsl", GL_VERTEX_SHADER);

        Shader frag;
        frag.loadFromFile("glsl/pass_through.f.glsl", GL_FRAGMENT_SHADER);

        wigglyShader.create();
        wigglyShader.attach(vert, frag);
        wigglyShader.link();
    }

    // Vulcano
    {
        Shader vert;
        vert.loadFromFile("glsl/Fountain/vert.glsl", GL_VERTEX_SHADER);

        Shader frag;
        frag.loadFromFile("glsl/Fountain/frag.glsl", GL_FRAGMENT_SHADER);

        ShaderProgram prog;
        prog.create();
        prog.attach(vert, frag);

        glBindAttribLocation(prog.handle(), 20, "a_lifespan");

        prog.link();

        vulSpout.program = prog;
        incSpell.program = prog;
    }


    glChk();
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initGLUT(&argc, argv);
    glewInit();
    printOpenGLInformation();

    initShaders();

    initTextures();
    initScene();

    start();

    return 0;
}
