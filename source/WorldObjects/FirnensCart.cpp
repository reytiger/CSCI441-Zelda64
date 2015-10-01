
#include "WorldObjects/FirnensCart.hpp"

/************************************************  DRAW FIRNENS CART
 * *************************************/
// the objects are a simple circle.
void FirnensCart::draw() const {
  glPushMatrix();
  {
    glColor3f(40 / 255.0, 40 / 255.0,
              40 / 255.0); // nice dark brown for the base
    // draw the body
    glPushMatrix();
    {
      glTranslatef(-0.5, 0, 0.75);
      glScalef(3, 2, 0.75);
      glutSolidCube(1);
    };
    glPopMatrix();
    // draw the wheels
    glPushMatrix();
    {
      glColor3f(134 / 255.0, 89 / 255.0,
                45 / 255.0); // nice dark brown for the base
      glRotatef(90, 1, 0, 0);
      glTranslatef(0, 0.5, 0);
      glRotatef(m_angleWheel, 0, 0, 1);
      glPushMatrix();
      {
        glTranslatef(0, 0, 1);
        glutSolidTorus(0.2, 0.5, 4, 8);
      };
      glPopMatrix();
      glPushMatrix();
      {
        glTranslatef(0, 0, -1);
        glutSolidTorus(0.2, 0.5, 4, 8);
      };
      glPopMatrix();
    };
    glPopMatrix();
  };
  glPopMatrix();
}
