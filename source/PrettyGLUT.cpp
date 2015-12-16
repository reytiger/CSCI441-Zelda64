#include "PrettyGLUT.hpp"

#include "Cameras.hpp"
#include "Shader.hpp"

#include <algorithm>
#include <fstream>

// We need to know about this. but it's entirely game logic so it's defined
// in main.cpp.
void updateScene(double t, double dt);

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

// Multi-pass shenanigans.
GLuint fbo;
constexpr size_t fboTexCount    = 4;
GLuint fboTextures[fboTexCount] = {};

static GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
};
static_assert(fboTexCount <= 10, "render::buffers is not large enough.");

// Give it that... retro feel.
GLsizei fbo_width  = 512;
GLsizei fbo_height = fbo_width;

// Things to draw
std::vector<WorldObject *> drawn = std::vector<WorldObject *>();

std::vector<RenderPass> renderPasses;

int passIdx = -1;

extern WorldObjModel level;
extern WorldObjModel kingRed;

// Call this after the swap buffer call to update the FPS, etc. counter.
// See: https://www.opengl.org/wiki/Performance#Measuring_Performance
void updateFrameCounter() {
    using namespace std::chrono;

    static auto last_updated = timer_clock::now();
    static auto then         = timer_clock::now();
    static int frames        = 0;

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
}

// TODO: Make this stroke.
void drawText(const std::string &text, Vec pos, Color color) {
    glDisable(GL_LIGHTING);
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
    // Make sure everything is drawn in front of everything.
    glTranslatef(0.0, 0.0, 1.0);

    // These two come from us using GLUT_BITMAP_9_BY_15 in drawText().
    static const size_t charWidth  = 9;
    static const size_t charHeight = 15;

    // What's the largest number we ever hope to see?
    static const size_t numLength = 12;
    static const size_t pixelsFromRight
        = (numLength + std::string(" ms / frame").size() + 1) * charWidth;

    static const size_t lineSpacing = charHeight;

    // FPS
    static const auto white = Color(1.0, 1.0, 1.0);
    auto pos = Vec(windowWidth - pixelsFromRight, windowHeight - lineSpacing);
    drawText(tfm::format("%*.1f FPS", numLength, live_fps), pos, white);

    // Frame time
    pos.y -= lineSpacing;
    std::string units = " s";
    auto frametime    = live_frametime;
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
    pos.y -= lineSpacing;
    drawText(tfm::format("%*d frames", numLength, live_frames), pos, white);

    pos.y -= lineSpacing;
    auto dims = tfm::format("%d x %d", fbo_width, fbo_height);
    pos.x += std::min(as<size_t>(0), numLength - dims.size()) * charWidth;

    drawText(tfm::format("%s resolution", dims), pos, white);

    glEnable(GL_LIGHTING);
}

void resize(int w, int h) {
    glChk();
    windowWidth  = w;
    windowHeight = h;

    // update the viewport to fill the window
    glViewport(0, 0, w, h);
    glChk();

    // update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, aspectRatio(), 0.1, 1e6);
    glChk();
}

void render() {
    glChk();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resize(windowWidth, windowHeight);

    glChk();

    // Render to our texture.
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glDrawBuffers(fboTexCount, buffers);
    glChk();

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, fbo_width, fbo_height);
    glChk();
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
        glEnable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);

        level.draw();
        pushMatrixAnd([&]() {
            glTranslatef(-5, 0, 0);
            // glScalef(0.005, 0.005, 0.005);
            kingRed.draw();
        });

        glEnable(GL_CULL_FACE);
        glChk();
    }

    glEnable(GL_TEXTURE_2D);
    for (size_t i = 0; i < fboTexCount; i += 1) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, fboTextures[i]);
    }
    glChk();

    if (passIdx >= 0) {
        renderPasses[passIdx].render();
    }

    glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTextures[0]);

    // Render the quad with our texture.
    glDrawBuffer(GL_BACK);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderProgram::useFFS();
    RenderPass::renderQuad();

    // Disable BEFORE the hud to avoid "out of bounds" errno
    glDisable(GL_TEXTURE_2D);

    // The HUD is separate.
    ShaderProgram::useFFS();
    renderHUD();

    glChk();

    // push the back buffer to the screen
    glutSwapBuffers();
    glChk();

    updateFrameCounter();
    glChk();
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

    const float q = 1 / 4.0f;
    const float t = 1 / 3.0f;

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
        "\\-----------------------------------------------------------------------/\n",
        // clang-format on
        glGetString(GL_VERSION),
        glGetString(GL_RENDERER),
        glGetString(GL_VENDOR));
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

    glChk();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glChk(); // Very odd errno "Directory not empty"

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

void initSkybox() {
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


//
// It's all callbacks from here.
//

// The int is requied, but unused.
void doFrame(int) {
    using namespace std::chrono;

    static const auto delay = milliseconds(1000 / FPS);
    glutTimerFunc(as<int>(delay.count()), doFrame, 0);

    static auto then = timer_clock::now();
    auto now         = timer_clock::now();
    auto dt          = now - then;
    then             = now;

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
            fudge = 0.0025f;
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

void nextShader() {
    passIdx += 1;
    if (passIdx >= renderPasses.size()) {
        passIdx = -1;
    }
}

void saveFrame(GLenum buffer, const std::string &suffix) {
    if (buffer > GL_COLOR_ATTACHMENT0 && buffer - GL_COLOR_ATTACHMENT0 < 10) {
        info("Saving image from buffer GL_COLOR_ATTACHMENT%d.",
             buffer - GL_COLOR_ATTACHMENT0);
    } else {
        info("Saving image from buffer #%d.", buffer);
    }

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(buffer);
    glChk();

    std::vector<GLubyte> bytes(3 * fbo_width * fbo_height);
    glReadPixels(
        0, 0, fbo_width, fbo_height, GL_RGB, GL_UNSIGNED_BYTE, bytes.data());
    glChk();

    std::ofstream out;

    // Write out as a PPM.
    auto now = timer_clock::now();
    out.open(tfm::format("image-%s_%s.ppm", 0.0, suffix));
    // clang-format off
    out << "P6\n"
        << fbo_width << " " << fbo_height << "\n"
        << 255 << std::endl;
    // clang-format on
    for (int row = 0; row < fbo_height; row += 1) {
        for (int col = 0; col < fbo_width; col += 1) {
            out << as<unsigned>(bytes[fbo_width * row + col]) << " ";
        }
        out << std::endl;
    }
}

void normalKeysDown(unsigned char key, int, int) {
    keyPressed[key] = true;

    switch (key) {
    case 27: // escape
        exit(0);
        break;
    case 'C':
    case 'c':
        if (activeCam == &freecam) {
            activeCam = &arcballcam;
            info("Switched to ArcBallCamera");
        } else if (activeCam == &arcballcam) {
            activeCam = &freecam;
            info("Switched to FreeCamera");
        }
        break;
    case '+':
        saveFrame(GL_FRONT, "front");
        saveFrame(buffers[0], "0");
        saveFrame(buffers[1], "1");
        saveFrame(buffers[2], "2");
        saveFrame(buffers[3], "3");
    // 0 turns off all passes.
    case '0':
        passIdx = -1;
    }

    // '1' turns off the 1st pass (idx 0), '2' the second, etc.
    if ('1' <= key && key <= '9') {
        int idx = key - '1';
        if (idx < renderPasses.size()) {
            passIdx = idx;
        } else {
            passIdx = -1;
        }
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
    info("Generated Framebuffer %s", fbo);
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

    glGenTextures(fboTexCount, fboTextures);

    for (size_t i = 0; i < fboTexCount; i += 1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboTextures[i]);
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

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               fboTextures[i],
                               0);
        glChk();
    }
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    // Unbind everything.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_TEXTURE_2D);

    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
        info("Framebuffer initialized completely!");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        fatal("Framebuffer failed to initialize completely! "
              "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        fatal("Framebuffer failed to initialize completely! "
              "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        fatal("Framebuffer failed to initialize completely! "
              "GL_FRAMEBUFFER_UNSUPPORTED");
        break;
    default:
        fatal("Framebuffer FAILED TO INITIALIZE COMPLETELY.");
        break;
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
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_FLAT);

    initSkybox();
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
