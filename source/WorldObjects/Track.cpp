#include <fstream>
#include "WorldObjects.hpp"

using namespace std;

Vec Track::eval_arc(double arc) { return m_halo.eval_arc(arc); }
Vec Track::eval_t(double t) { return m_halo.eval_t(t); }

Vec Track::eval_deriv_arc(double arc) { return m_halo.eval_deriv_arc(arc); }
Vec Track::eval_deriv_t(double t) { return m_halo.eval_deriv_t(t); }

/*************************  DRAW WORLD SURFACE  *******************************/
void Track::draw() const {
    pushMatrixAnd([&]() {
        m_halo.draw();
        m_trackCallBack.draw();
    });
}

void Track::drawTrackBlocks() const {
    double stepSize = 0.01;
    pushMatrixAnd([&]() {
        glDisable(GL_CULL_FACE);
        glBegin(GL_TRIANGLES);
        {
            for (double i = 0; i < 1 + stepSize; i += stepSize) {
                Vec current = m_halo.eval_t(i);
                Vec target  = m_halo.eval_t(i + stepSize);

                auto mat = Material::random();
                mat.emission(mat.diffuse());
                mat.set();

                // 1st triangle
                glVertex3d(current.x, current.y, current.z);
                glVertex3d(current.x + 1, current.y, current.z);
                glVertex3d(target.x, target.y, target.z);

                // 2nd triangle
                glVertex3d(target.x, target.y, target.z);
                glVertex3d(current.x + 1, current.y, current.z);
                glVertex3d(target.x + 1, target.y, target.z);
            }
        };
        glEnd();
        glEnable(GL_CULL_FACE);
    });
}

/*************************  SET UP FOR CURVES  ********************************/
////////////////////////////////////////////////////////////////////////////////
void Track::init() {
    m_trackCallBack = CallListObject([this](GLuint dl) {
        glNewList(dl, GL_COMPILE);
        drawTrackBlocks();
        glEndList();
    });
}

double Track::evalAngle(double t) { return fmod(t, 1) * 360.0; }
