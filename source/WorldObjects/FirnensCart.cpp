
#include "WorldObjects/FirnensCart.hpp"
#include "WorldObjects/Firnen.hpp"

Firnen test;

/************************************************  DRAW FIRNENS CART
 * *************************************/
// the objects are a simple circle.
void FirnensCart::draw() const {
    glPushMatrix();
    {
        glRotated(-90, 1, 0, 0);
        // glPushMatrix();
        // { PrettyGLUT::drawn.push_back(&firnen); };
        // glPopMatrix();

        glColor3d(40 / 255.0,
                  40 / 255.0,
                  40 / 255.0); // nice dark brown for the base
        // draw the body
        glPushMatrix();
        {
            glTranslated(-0.5, 0, 0.75);
            glScaled(3, 2, 0.75);
            glutSolidCube(1);
        };
        glPopMatrix();
        // draw the wheels
        glPushMatrix();
        {
            glColor3d(134 / 255.0,
                      89 / 255.0,
                      45 / 255.0); // nice dark brown for the base
            glRotated(90, 1, 0, 0);
            glTranslated(0, 0.5, 0);
            glRotated(m_wheelAngle, 0, 0, 1);
            glPushMatrix();
            {
                glTranslated(0, 0, 1);
                glutSolidTorus(0.2, 0.5, 4, 8);
            };
            glPopMatrix();
            glPushMatrix();
            {
                glTranslated(0, 0, -1);
                glutSolidTorus(0.2, 0.5, 4, 8);
            };
            glPopMatrix();
        };
        glPopMatrix();
    };
    glPopMatrix();
}
