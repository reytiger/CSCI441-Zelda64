#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include <fstream>

CallListObject roomFloor;
CallListObject vulcano;

paone::Object venus;
paone::Object temple;
PointLight light;

Incallidus inc;

Texture grass;
Texture skybox;

Incallidus enemies[2];

PacmanGame game;

float vulHeight     = 50.0f;
float vulBaseRadius = 30.0f;

static const auto floorHalfSize = Vec(100, 100);

// Returns a copy of 'str' with leading and trailing whitespace removed.
std::string trim(std::string str) {
    auto pred = std::ptr_fun<int, int>(std::isspace);
    // From the left.
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), pred));
    // From the right.
    str.erase(std::find_if_not(str.rbegin(), str.rend(), pred).base(),
              str.end());
    return str;
}

// 'lo' and 'hi' use x and y, but pos is in x and z.
// 'y' is passed in to define the plain.
bool inRect(Vec pos, float y, Vec lo, Vec hi) {
    return pos.y >= y && (lo.x <= pos.x && pos.x <= hi.x)
           && (lo.y <= pos.z && pos.z <= hi.y);
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

    wigglyShader.attachUniform("time", 1000.0 + t);
    static bool jumping = false;

    if (jumping) {
        if (inc.pos().y > 0) {
            inc.moveByY(dt * -9.8f * 10);
        } else {
            jumping = false;
            inc.moveToY(0.5f);
        }
    } else if (!inRect(inc.pos(), 0.0f, -floorHalfSize, floorHalfSize)) {
        inc.moveByY(dt * -9.8f * 10); // fudge factor
    }

    if (inc.pos().y < -500.0f) {
        game.endGame();
    }

    for (WorldObject *wo : drawn) {
        wo->update(t, dt);
    }

    if (!jumping && keyPressed[' ']) {
        jumping = true;
        // Truely a lazy jump.
        inc.moveByY(100);
    }

    game.update(t, dt);
}

void initScene() {
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Color(1.0, 1.0, 1.0).v);

    // Global constructors do weird things.
    inc = Incallidus();

    // Light
    drawn.push_back(&light);
    light.enable();
    light.moveToY(5.0);
    light.setUpdateFunc([&](double t, double /*dt*/) {
        t /= 5.0;
        auto color = Color(cos(3.0 * t), cos(5.0 * t), cos(4.0 * t));
        color = 0.5f * color + 0.5f;
        light.ambient(color.v);
        light.diffuse(color.v);
        light.specular(color.v);
    });

    // Floor
    drawn.push_back(&roomFloor);
    roomFloor = CallListObject([&](GLuint dl) {
        glNewList(dl, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_CULL_FACE);
        glBindTexture(GL_TEXTURE_2D, grass);

        static const auto texsize = 0.1f * floorHalfSize;

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 1.0f, 0.0f);

        glTexCoord2f(-texsize.x, texsize.y);
        glVertex3d(-floorHalfSize.x, 0, floorHalfSize.y);

        glTexCoord2f(texsize.x, texsize.y);
        glVertex3d(floorHalfSize.x, 0, floorHalfSize.y);

        glTexCoord2f(texsize.x, -texsize.y);
        glVertex3d(floorHalfSize.x, 0, -floorHalfSize.y);

        glTexCoord2f(-texsize.x, -texsize.y);
        glVertex3d(-floorHalfSize.x, 0, -floorHalfSize.y);

        glEnd();

        glDisable(GL_TEXTURE_2D);
        glEndList();
    });
    glChk();

    // Vulcano
    // drawn.push_back(&vulcano);
    // vulcano.moveTo(-50, 0, -50);
    // vulcano.shader(wigglyShader);

    // static auto vulcano_body = gluNewQuadric();
    // static auto vulcano_top  = gluNewQuadric();

    // vulcano = CallListObject([&](GLuint dl) {Nico found a neat technique where you fall off
    //     glNewList(dl, GL_COMPILE);
    //     glDisable(GL_CULL_FACE);

    //     pushMatrixAnd([&]() {
    //         glRotatef(-90.0f, 1, 0, 0);
    //         gluCylinder(vulcano_body,
    //                     vulBaseRadius,
    //                     vulBaseRadius / 4.0,
    //                     vulHeight,
    //                     20,
    //                     20);
    //     });

    //     pushMatrixAnd([&]() {
    //         glTranslatef(0.0f, vulHeight - 0.25f, 0.0f);
    //         glRotatef(90.0f, -1, 0, 0);
    //         gluDisk(vulcano_top, 0, vulBaseRadius / 4.0, 20, 1);
    //     });

    //     glEndList();
    // });

    // Our Hero!
    drawn.push_back(&inc);
    inc.setUpdateFunc([&](double t, double /*dt*/) {
        inc.doWASDControls(30.0, keyPressed, false);
        inc.radius(3.5f + 1.0f * cosf(t));
    });

    game.initScene(&inc);

    // Camera
    activeCam->follow(&inc);
    activeCam->radius(150.0);
    activeCam->rotate(0.0f, PI);

    // Venus 1
    auto pt = venus.getLocation();
    assert(pt);
    pt->setY(8.7);
    pt->setX(-50);
    pt->setZ(50);
    venus.loadObjectFile("assets/venus.obj");

    // Temple
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

    // Lit planes - like the ground!
    {
        Shader vert;
        vert.loadFromFile("glsl/Ground/vert.glsl", GL_VERTEX_SHADER);

        Shader frag;
        frag.loadFromFile("glsl/Ground/frag.glsl", GL_FRAGMENT_SHADER);

        ShaderProgram prog;
        prog.create();
        prog.attach(vert, frag);
        prog.link();

        roomFloor.shader(prog);
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
