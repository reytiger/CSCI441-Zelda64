#include "PrettyGLUT.hpp"

Incallidus inc;

// This function is expected by PrettyGLUT, because I designed it to get done
// fast, not smart. We can change this later, but this makes sure it builds.
// It takes in t and dt, the time and time since the last updateScene was
// called.
void updateScene(double t, double dt) {
    defaultCamera.update(t, dt);
    inc.update(t, dt);
}

void initScene() {
    float lightCol[4]   = {1, 1, 1, 1};
    float ambientCol[4] = {0.0, 0.0, 0.0, 1.0};
    float lPosition[4] = {10, 10, 10, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);

    // tell OpenGL not to use the material system; just use whatever we
    // pass with glColor*()
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Load up Incallidus!
    PrettyGLUT::drawn.push_back(&inc);
    inc.setRadius(10.0);
}

int main(int argc, char **argv) {
    srand(time(nullptr));

    PrettyGLUT::initGLUT(&argc, argv);
    PrettyGLUT::printOpenGLInformation();
    initScene();
    PrettyGLUT::start();
    return 0;
}
