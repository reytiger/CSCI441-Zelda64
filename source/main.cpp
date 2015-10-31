#include "PrettyGLUT.hpp"
#include "WorldObjects.hpp"

#include <array>

CallListObject roomFloor;
paone::Object model;
PointLight light;

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
    activeCam->doWASDControls(25.0, keyPressed, true);
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

        static const auto size = Vec(100, 100);

        for (int i = -1; i <= size.x + 1; i += 1) {
            glBegin(GL_TRIANGLE_STRIP);
            // The terrain is flat. All of the normals are straight up.
            glNormal3f(0.0, 1.0, 0.0);
            for (int k = -1; k <= size.y; k += 1) {
                Vec off  = Vec(2.0, 2.0);
                auto pos = Vec(i, k) * off - Vec(200.0, 200.0) / 2.0;

                Material::GreenRubber.set();

                glVertex3d(pos.x, 0.0, pos.y);
                glVertex3d(pos.x, 0.0, pos.y - off.y);
                glVertex3d(pos.x - off.x, 0.0, pos.y);

                // Skip the second triangle on extreme cases.
                // TODO: This is probably wrong.
                if (k == size.y || i == size.x) {
                    break;
                }

                glVertex3d(pos.x, 0.0, pos.y);
                glVertex3d(pos.x, 0.0, pos.y + off.y);
                glVertex3d(pos.x + off.x, 0.0, pos.y);
            }
            glEnd();
        }

        glEndList();
    });
    roomFloor.moveToY(-4.7);

    activeCam->moveToY(20.0f);

    model.loadObjectFile("assets/venus.obj");
}

int main(int argc, char **argv) {
    errno = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    initGLUT(&argc, argv);

    printOpenGLInformation();

    initScene();

    start();

    return 0;
}
