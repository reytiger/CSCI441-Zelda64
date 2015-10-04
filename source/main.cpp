#include "PrettyGLUT.hpp"

#include "WorldObjects.hpp"

Incallidus inc;
BezierCurve halo;
CallListObject roomFloor;
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

    PrettyGLUT::drawn.push_back(&halo);
    halo.loadFile("./assets/world/bezier-halo.csv");
    halo.moveToZ(1.0);

    defaultCamera.setUpdateFunc([](double t, double dt) {
        Vec vel;

        Vec forward = defaultCamera.lookAt();
        Vec right   = -defaultCamera.up().cross(forward);

        const auto cameraSpeed = dt * 50.0;

        if (PrettyGLUT::keyPressed['d']) {
            vel += right;
        }
        if (PrettyGLUT::keyPressed['a']) {
            vel -= right;
        }
        if (PrettyGLUT::keyPressed['w']) {
            vel += forward;
        }
        if (PrettyGLUT::keyPressed['s']) {
            vel -= forward;
        }

        // If no keys were pressed, vel == (0, 0) and we can't normalize.
        if (vel.norm()) {
            vel = cameraSpeed * vel.normalize();
        }

        defaultCamera.setVelocity(vel);
    });
    defaultCamera.moveToY(1.0);

    PrettyGLUT::drawn.push_back(&roomFloor);
    roomFloor = CallListObject([](GLuint dl) {
        glNewList(dl, GL_COMPILE);
        auto citySize = Vec(100, 100);
        glColor3d(0.3, 0.6, 0.3);

        for (int i = -10; i <= 100 + 10; i += 1) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int k = -10; k <= 100 + 10; k += 1) {
                Vec off  = citySize / 100;
                auto pos = Vec(i, k) * off - citySize / 2.0;

                // glColor3d(getRand(), getRand(), getRand());
                glVertex3d(pos.x, 0.0, pos.y);
                glVertex3d(pos.x, 0.0, pos.y - off.y);
                glVertex3d(pos.x - off.x, 0.0, pos.y);

                // glColor3d(getRand(), getRand(), getRand());
                glVertex3d(pos.x, 0.0, pos.y);
                glVertex3d(pos.x, 0.0, pos.y + off.y);
                glVertex3d(pos.x + off.x, 0.0, pos.y);
            }
            glEnd();
        }

        glEndList();
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
