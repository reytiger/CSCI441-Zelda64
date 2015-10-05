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
    for (WorldObject *wo : PrettyGLUT::drawn) {
        wo->update(t, dt);
    }

    // Even though they're rendered, the cameras are NOT in the drawn list.
    PrettyGLUT::activeCam->update(t, dt);
}

void initScene() {
    float lightCol[4]   = {1, 1, 1, 1};
    float ambientCol[4] = {0.0, 0.0, 0.0, 1.0};
    float lPosition[4] = {0.0, 0.0, 0.0, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);

    // tell OpenGL not to use the material system; just use whatever we
    // pass with glColor*()
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Cameras are important. Note, they are hard-coded in the render loop.
    // Do NOT add a camera to the  PrettyGLUT::drawn, or it'll draw (and update)
    // twice.

    // Setup controls for PrettyGLUT::freecam.
    PrettyGLUT::freecam.addWASDControls(100.0, PrettyGLUT::keyPressed);
    PrettyGLUT::freecam.moveToY(1.0);

    // Cam2 is much faster.
    PrettyGLUT::freecam2.addWASDControls(500.0, PrettyGLUT::keyPressed);
    PrettyGLUT::freecam2.moveToY(1.0);

    PrettyGLUT::arcballcam.setRadius(10.0);
    // PrettyGLUT::arcballcam.addArcBallControls(???);

    // Load up Incallidus!
    PrettyGLUT::drawn.push_back(&inc);
    inc.setRadius(0.1);

    PrettyGLUT::drawn.push_back(worldSurface);
    worldSurface = new WorldSurface();

    PrettyGLUT::drawn.push_back(&halo);
    halo.loadFile("./assets/world/bezier-halo.csv");
    halo.moveToZ(5.0);
    halo.setUpdateFunc([](double t, double dt) {
        auto theta = t;
        halo.moveTo(Vec(cos(1.1 * theta),
                        sin(1.3 * theta),
                        cos(1.5 * theta) * sin(0.9 * theta)));
    });

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
