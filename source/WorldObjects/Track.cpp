#include <fstream>
#include "WorldObjects/Track.hpp"

using namespace std;

/*************************  DRAW WORLD SURFACE  *******************************/
void Track::draw() const {
    pushMatrixAnd([&]() {
        m_halo.draw();
        drawTrackBlocks();
    });
    // pushMatrixAnd([&]() {
    //     Point test = eval(-1, -1.8);
    //     glPushMatrix();
    //     {
    //         glColor3d(200 / 255.0, 10 / 255.0, 10 / 255.0);
    //         glTranslated(test.getX(), test.getY(), test.getZ());
    //         glScaled(0.2, 0.2, 0.2);
    //         glutSolidSphere(1, 10, 10);
    //     };
    //     glPopMatrix();

    // });
    // pushMatrixAnd([&]() { drawGround(); });
}

void Track::drawTrackBlocks() const {
    pushMatrixAnd([&]() {
        Vec target = m_halo.eval(0.0001);
        glColor3d(200 / 255.0, 200 / 255.0, 200 / 255.0);
        glTranslated(target.x, target.y, target.z);
        glTranslated(0, 1, 0);
        glutSolidCube(1);
    });
}

/*************************  SET UP FOR CURVES  ********************************/
////////////////////////////////////////////////////////////////////////////////
//  Load our control points from file and store them in a global variable.
