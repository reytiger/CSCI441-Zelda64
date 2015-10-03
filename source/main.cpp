#include "PrettyGLUT.hpp"

#include "WorldObjects.hpp"

Incallidus inc;
BezierCurve curve;

// Global
WorldSurface *worldSurface;

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
    inc.setRadius(0.1);

    PrettyGLUT::drawn.push_back(worldSurface);
    worldSurface = new WorldSurface();

    PrettyGLUT::drawn.push_back(&curve);
    curve.loadFile("./assets/world/bezier-halo.csv");

    defaultCamera.setUpdateFunc([](double t, double dt) {
        Vec vel;

        const auto heroWalkSpeed = dt * 5.0;

        if (PrettyGLUT::keyPressed['d']) {
            vel.x += heroWalkSpeed;
        }
        if (PrettyGLUT::keyPressed['a']) {
            vel.x -= heroWalkSpeed;
        }
        if (PrettyGLUT::keyPressed['w']) {
            vel.z += heroWalkSpeed;
        }
        if (PrettyGLUT::keyPressed['s']) {
            vel.z -= heroWalkSpeed;
        }

        defaultCamera.setVelocity(vel);
    });
}

int main(int argc, char **argv) {
    srand(time(nullptr));

    PrettyGLUT::initGLUT(&argc, argv);
    PrettyGLUT::printOpenGLInformation();
    initScene();
    PrettyGLUT::start();
    return 0;
}
