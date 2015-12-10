#include "PrettyGLUT.hpp"

#include "Cameras.hpp"
#include "Shader.hpp"

// We need to know about this. but it's entirely game logic so it's defined
// in main.cpp.
void updateScene(double t, double dt);

Texture grass;
Texture skybox;
Texture loading;

// Cameras
FreeCamera freecam;
ArcBallCamera arcballcam;

// World objects
Camera *activeCam = &freecam;

double live_fps       = 0.0;
double live_frametime = 0.0;
int live_frames       = 0;

// Display Settings
int windowWidth  = 1280;
int windowHeight = 1024;

Color colorClear = Color(48, 24, 96);

// Input states
Vec mouse             = Vec();
int leftMouse         = 0;
GLint modifiersButton = 0;
bool keyPressed[256]  = {};

// Multipass shenanigans.
GLuint fbo;
GLuint fboTex;

// Give it that... retro feel.
size_t fbo_width  = 512;
size_t fbo_height = fbo_width;

// Things to draw
std::vector<WorldObject *> drawn = std::vector<WorldObject *>();
// extern paone::Object levelBongo;
extern paone::Object levelHyruleField;

// TODO: Make this stroke.
void drawText(const std::string &text, Vec pos, Color color) {
    glColor3fv(color.v);
    glRasterPos2d(pos.x, pos.y);
    pushMatrixAnd([&]() {
        for (size_t i = 0; i < text.size(); i += 1) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
        }
    });
}

void renderHUD() {
    glDisable(GL_LIGHTING);
    // Switch to 2D.
    // TODO: Preserve matrices properly.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Make sure everything is drawn infront of everything.
    glTranslatef(0.0, 0.0, 1.0);

    // These two come from us using GLUT_BITMAP_9_BY_15 in drawText().
    static const size_t charWidth  = 9;
    static const size_t charHeight = 15;

    // 10 digits seems reasonable for "What's the largest number we ever hope to
    // see?".
    static const size_t numLength = 10;
    static const size_t pixelsFromRight
        = (numLength + std::string(" ms / frame").size() + 1) * charWidth;

    static const size_t lineSpacing = charHeight;

    // FPS
    auto white = Color(1.0, 1.0, 1.0);
    auto pos = Vec(windowWidth - pixelsFromRight, windowHeight - lineSpacing);
    drawText(tfm::format("%*.1f FPS", numLength, live_fps), pos, white);

    // Frame time
    pos               = pos - Vec(0, lineSpacing);
    std::string units = "??";
    auto frametime = live_frametime;
    if (frametime < 1e-6) {
        units = "ns";
        frametime *= 1e9;
    } else if (frametime < 1e-3) {
        units = "us";
        frametime *= 1e6;
    } else if (frametime < 1) {
        units = "ms";
        frametime *= 1e3;
    }

    std::string frametime_text
        = tfm::format("%*.2f %s / frame", numLength, frametime, units);
    drawText(frametime_text, pos, white);

    // Frame count
    pos = pos - Vec(0, lineSpacing);
    drawText(tfm::format("%*d frames", numLength, live_frames), pos, white);

    glEnable(GL_LIGHTING);
}

void resize(int w, int h) {
    windowWidth  = w;
    windowHeight = h;

    // update the viewport to fill the window
    glViewport(0, 0, w, h);

    // update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, aspectRatio(), 0.1, 1e6);
}

void render() {
    glDrawBuffer(GL_BACK);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resize(windowWidth, windowHeight);

    // Render to our texture.
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, fbo_width, fbo_height);
    {
        glClearColor(colorClear.r, colorClear.g, colorClear.b, colorClear.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        activeCam->adjustGLU();

        pushMatrixAnd([&]() {
            auto scale = 1000.0f;
            glScalef(scale, scale, scale);
            void renderSkybox();
            renderSkybox();
        });

        glChk();
        for (WorldObject *wo : drawn) {
            glChk();
            wo->draw();
            glChk();
        }

        ShaderProgram::useFFS();
        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);

        // levelBongo.draw();
        levelHyruleField.draw();

        glEnable(GL_CULL_FACE);
        glEnable(GL_LIGHTING);

        glChk();
    }

    glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: Subsequent passes.

    // Render the quad with our texture.
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    pushMatrixAnd([]() {
        glLoadIdentity();

        double ratio = 2.0f * fbo_height / double(fbo_width);

        gluOrtho2D(0.0, 1.0, 0.0, ratio);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, fboTex);

        glBegin(GL_QUADS);
        {
            float x[] = {0.0, 1.0, 1.0, 0.0};
            float y[] = {0.0, 0.0, 1.0, 1.0};

            glTexCoord2f(0.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, ratio, 0.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(0.0, ratio, 0.0);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);

        // Make sure we pop the right matrix.
        glMatrixMode(GL_PROJECTION);
    });

    // The HUD is separate.
    ShaderProgram::useFFS();
    renderHUD();

    // push the back buffer to the screen
    glutSwapBuffers();
}


void renderSkybox() {
    ShaderProgram::useFFS();
// Coordinates for each face of the skybox within the skybox texture.
// "Forward" is the leftmost square in the texture and "bottom" is the one
// immediately to the right.
#define _(q, t) Vec(q / 4.0, t / 3.0)
    auto left   = _(0, 1);
    auto front  = _(1, 1);
    auto right  = _(2, 1);
    auto back   = _(3, 1);
    auto top    = _(1, 2);
    auto bottom = _(1, 0);
#undef _

    const auto q = 1 / 4.0;
    const auto t = 1 / 3.0;

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, skybox);

    pushMatrixAnd([&]() {
        glBegin(GL_QUADS);

        auto src = top;

        glTexCoord2f(src.x, src.y);
        glVertex3d(-1, 1, -1);

        glTexCoord2f(src.x + q, src.y);
        glVertex3d(1, 1, -1);

        glTexCoord2f(src.x + q, src.y + t);
        glVertex3d(1, 1, 1);

        glTexCoord2f(src.x, src.y + t);
        glVertex3d(-1, 1, 1);

        glEnd();
    });

    pushMatrixAnd([&]() {
        glBegin(GL_QUADS);

        auto src = bottom;

        glTexCoord2f(src.x, src.y + t);
        glVertex3d(-1, -1, 1);

        glTexCoord2f(src.x, src.y);
        glVertex3d(1, -1, 1);

        glTexCoord2f(src.x + q, src.y);
        glVertex3d(1, -1, -1);

        glTexCoord2f(src.x + q, src.y + t);
        glVertex3d(-1, -1, -1);

        glEnd();
    });

    pushMatrixAnd([&]() {
        glBegin(GL_QUADS);

        auto src = left;

        glTexCoord2f(src.x + q, src.y + t);
        glVertex3d(-1, 1, -1);

        glTexCoord2f(src.x, src.y + t);
        glVertex3d(-1, 1, 1);

        glTexCoord2f(src.x, src.y);
        glVertex3d(-1, -1, 1);

        glTexCoord2f(src.x + q, src.y);
        glVertex3d(-1, -1, -1);

        glEnd();
    });

    pushMatrixAnd([&]() {
        glBegin(GL_QUADS);

        auto src = right;

        glTexCoord2f(src.x, src.y);
        glVertex3d(1, -1, -1);

        glTexCoord2f(src.x + q, src.y);
        glVertex3d(1, -1, 1);

        glTexCoord2f(src.x + q, src.y + t);
        glVertex3d(1, 1, 1);

        glTexCoord2f(src.x, src.y + t);
        glVertex3d(1, 1, -1);

        glEnd();
    });

    pushMatrixAnd([&]() {
        glBegin(GL_QUADS);

        auto src = front;

        glTexCoord2f(src.x + q, src.y);
        glVertex3d(1, -1, -1);

        glTexCoord2f(src.x + q, src.y + t);
        glVertex3d(1, 1, -1);

        glTexCoord2f(src.x, src.y + t);
        glVertex3d(-1, 1, -1);

        glTexCoord2f(src.x, src.y);

        glVertex3d(-1, -1, -1);

        glEnd();
    });

    pushMatrixAnd([&]() {
        glBegin(GL_QUADS);

        auto src = back;

        glTexCoord2f(src.x + q, src.y);
        glVertex3d(-1, -1, 1);

        glTexCoord2f(src.x + q, src.y + t);
        glVertex3d(-1, 1, 1);

        glTexCoord2f(src.x, src.y + t);
        glVertex3d(1, 1, 1);

        glTexCoord2f(src.x, src.y);
        glVertex3d(1, -1, 1);

        glEnd();
    });

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void printOpenGLInformation() {
    GLint attribs = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribs);

    GLint lights = 0;
    glGetIntegerv(GL_MAX_LIGHTS, &lights);

    info(
        // We don't want to break this string across lines.
        // clang-format off
         "\n"
         "/-----------------------------------------------------------------------\\\n"
         "| OpenGL Information                                                    |\n"
         "|-----------------------------------------------------------------------|\n"
         "|   OpenGL Version:  %50s |\n"
         "|   OpenGL Renderer: %50s |\n"
         "|   OpenGL Vendor:   %50s |\n"
         "|-----------------------------------------------------------------------|\n"
         "|   Max Vert Attrib: %50s |\n"
         "|   Max GL Lights:   %50s |\n"
        "\\-----------------------------------------------------------------------/\n",
        // clang-format on
        glGetString(GL_VERSION),
        glGetString(GL_RENDERER),
        glGetString(GL_VENDOR),
        attribs,
        lights);
}

void loadLoadingScreen() {
    glChk();
    loading = SOIL_load_OGL_texture(
        "assets/textures/Legend-of-Zelda-Ocarina-of-Time-Title-Screen.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
            | SOIL_FLAG_COMPRESS_TO_DXT);
    glChk();
    {
        glBindTexture(GL_TEXTURE_2D, loading);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glChk();
}

void renderLoadingScreen() {
    if (loading == 0) {
        info("Loading 'loading screen'.");
        loadLoadingScreen();
    }
    glChk();
    glDisable(GL_LIGHTING);

    glDrawBuffer(GL_FRONT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glChk();

    glMatrixMode(GL_PROJECTION);
    pushMatrixAnd([&]() {
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, loading);
        glChk();

        glMatrixMode(GL_MODELVIEW);
        pushMatrixAnd([&]() {
            glLoadIdentity();
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);

            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(1.0f, 0.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(1.0f, 1.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0.0f, 1.0f);

            glEnd();
        });

        glMatrixMode(GL_PROJECTION);
    });
    glChk();
}

//
// It's all callbacks from here.
//

// The int is requied, but unused.
void doFrame(int) {
    using namespace std::chrono;

    static const auto delay  = milliseconds(1000 / FPS);
    static auto last_updated = timer_clock::now();
    static auto then         = timer_clock::now();
    static int frames        = 0;

    // Register the next update ASAP. We want this timing to be as consistent
    // as we can get it to be.
    glutTimerFunc(as<int>(delay.count()), doFrame, 0);

    frames += 1;

    auto now = timer_clock::now();
    auto dt  = now - then;
    then     = now;

    // Keep a live, running average FPS counter.
    if (now - last_updated > FPS_update_delay) {
        live_fps       = frames / duration<double>(now - last_updated).count();
        live_frametime = duration<double>(dt).count() / frames;
        live_frames    = frames;

        last_updated = now;
        frames       = 0;
    }

    // TODO: Pass a t which changes. We're pushing out all of our precision
    // with really big numbers.
    static double running = 0.0;
    running += duration<double>(dt).count();
    updateScene(running, duration<double>(dt).count());

    glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
    // update the left mouse button states, if applicable
    switch (button) {
    case GLUT_LEFT_BUTTON:
        mouse           = Vec(x, y, 0);
        leftMouse       = state;
        modifiersButton = glutGetModifiers();
        break;
    }
}

void mouseMotion(int x, int y) {
    if (leftMouse == GLUT_DOWN) {
        float fudge = 0.002f;

        int dx = static_cast<int>(mouse.x) - x;
        int dy = static_cast<int>(mouse.y) - y;

        // Arcball feels more natural with the y inverted and more fudge.
        if (dynamic_cast<ArcBallCamera *>(activeCam)) {
            fudge = 0.005f;
        }

        mouse.x = as<float>(x);
        mouse.y = as<float>(y);

        // Adjust the radius of the active cam. Moves by a constant factor of
        // the idstance of the mouse moved.
        if (modifiersButton == GLUT_ACTIVE_CTRL) {
            // Different controls, different fudge factor.
            fudge *= 15.0f;

            // info("%s", dx);
            Vec dist    = Vec(dx, dy);
            auto radius = activeCam->radius();
            if (dy > 0) {
                radius = radius - fudge * dist.norm();
            } else {
                radius = radius + fudge * dist.norm();
            }
            radius = clamp(radius, 3.0f, 1000.0f);
            activeCam->radius(radius);
            return;
        }

        // Arcball feels more natural with the y inverted and more fudge.
        if (dynamic_cast<ArcBallCamera *>(activeCam)) {
            activeCam->rotate(fudge * dx, -fudge * dy);
        } else {
            activeCam->rotate(fudge * dx, fudge * dy);
        }
    }
}

void normalKeysDown(unsigned char key, int, int) {
    keyPressed[key] = true;

    switch (key) {
    case 27: // escape
        exit(0);
    }
}

void normalKeysUp(unsigned char key, int, int) {
    keyPressed[key] = false;

    switch (key) {
        // Do nothing (yet?)
    }
}

void initFBO() {
    glChk();

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glChk();

    GLuint renderbuff;
    glGenRenderbuffers(1, &renderbuff);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuff);
    glChk();

    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbo_width, fbo_height);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuff);
    glChk();

    glGenTextures(1, &fboTex);
    glBindTexture(GL_TEXTURE_2D, fboTex);
    glChk();

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glChk();

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 fbo_width,
                 fbo_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 NULL);
    glChk();
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
    glChk();
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    // Unbind everything.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_TEXTURE_2D);

    if (status == GL_FRAMEBUFFER_COMPLETE) {
        info("Framebuffer initialized completely!");
    } else {
        info("Framebuffer FAILED TO INITIALIZE COMPLETELY.");
    }
}

// This sets up anything GLUT. This more or less never needs to change.
void initGLUT(int *argcp, char **argv) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(windowWidth, windowHeight);

    glutCreateWindow(windowTitle);

    // Display this until we start rendering "for real".
    renderLoadingScreen();

    // Our boat-load of callbacks.
    glutKeyboardFunc(normalKeysDown);
    glutKeyboardUpFunc(normalKeysUp);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);

    // Misc. options
    glEnable(GL_DEPTH_TEST);
    // Since we keep track of whether keys are up or down, we don't want to
    // spam the event.
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    // Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_FLAT);

    glDisable(GL_COLOR_MATERIAL);
}

void initOpenGL(int *argcp, char **argv) {
    initGLUT(argcp, argv);

    glewInit();
    initFBO();
}

void start() {
    doFrame(0);

    glutMainLoop();
}
