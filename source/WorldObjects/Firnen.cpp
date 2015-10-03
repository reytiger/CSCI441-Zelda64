
#include "WorldObjects/Firnen.hpp"

// Firnen::Firnen() { m_armRotation = 0.0; }

/************************************************  DRAW FIRNEN
 * *************************************/
// the objects are a simple circle.
void Firnen::draw() const {
    if (!m_visible) {
        return;
    }
    glPushMatrix();
    {
        glScalef(0.7, 0.7, 0.7); // make him smaller
        glPushMatrix();
        {
            glTranslatef(0, 1, 0.5);
            // drawLeg();
        };
        glPopMatrix();
        glPushMatrix();
        {
            glScalef(1, -1, 1);
            glTranslatef(0, 1, 0.5);
            // drawLeg();
        };
        glPopMatrix();
        glScalef(1, 1, 1 - (m_height * 0.1));
        drawTorso();
        // glutSolidCone( 1, height, 5, 2 );
    };
    glPopMatrix();
}

void Firnen::drawTorso() const {
    glPushMatrix();
    {
        glTranslatef(0, 0, 2);
        // body first
        drawBody();
        // left arm
        glPushMatrix();
        {
            glTranslatef(0, 1, 0.5);
            glRotatef(m_armRotation * 20, 0, 1, 0);
            drawArm();
        };
        glPopMatrix();
        // right arm
        glPushMatrix();
        {
            glScalef(1, -1, 1);
            glTranslatef(0, 1, 0.5);
            glRotatef(-m_armRotation * 20, 0, 1, 0);
            drawArm();
        };
        glPopMatrix();
        // draw the head
        glTranslatef(0, 0, 1.25);
        glScalef(0.8, 0.8, 0.8);
        drawHead();
    };
    glPopMatrix();
}

void Firnen::drawArm() const {
    glPushMatrix();
    {
        glColor3f(219 / 255.0, 150 / 255.0, 105 / 255.0); //
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        glRotatef(35, 1, 0, 0);
        gluCylinder(quadratic, 0.2, 0.15, 0.5, 20, 2); // top arm
        glTranslatef(0, 0, -0.6);
        gluCylinder(quadratic, 0.15, 0.2, 0.6, 20, 2); // top arm
        glRotatef(-40, 0, 1, 0);
        glTranslatef(0, 0, -0.4);
        gluCylinder(quadratic, 0.2, 0.15, 0.5, 20, 2); // lower arm
        glTranslatef(0, 0, -0.5);
        gluCylinder(quadratic, 0.15, 0.2, 0.5, 20, 2); // lower arm
    };
    glPopMatrix();
}

void Firnen::drawBody() const {
    glPushMatrix();
    {
        glColor3f(153 / 255.0, 153 / 255.0, 45 / 255.0); // nice green tunic
        glScalef(1, 1.2, 1.5);
        glutSolidSphere(1, 10, 10);
    };
    glPopMatrix();
}

void Firnen::drawHead() const {
    // 0, 0 should be the base of the neck
    glPushMatrix();
    {
        glColor3f(219 / 255.0, 150 / 255.0, 105 / 255.0); //
        glPushMatrix();
        {
            GLUquadricObj *quadratic;
            quadratic = gluNewQuadric();
            gluCylinder(quadratic, 0.5, 0.3, 1, 20, 2);
        };
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(0, 0, 1);
            glutSolidSphere(0.9, 10, 10);
            glPushMatrix();
            {
                glTranslatef(0.65, 0.22, 0);
                glRotatef(-80, 0, 0, 1);
                drawEye();
            };
            glPopMatrix();
            glPushMatrix();
            {
                glTranslatef(0.65, -0.22, 0);
                glRotatef(-90, 0, 0, 1);
                drawEye();
            };
            glPopMatrix();
        };
        glPopMatrix();
    };
    glPopMatrix();
}

void Firnen::drawEye() const {
    glPushMatrix();
    {
        glColor3f(0 / 255.0, 0 / 255.0, 0 / 255.0); // black
        glutSolidSphere(0.3, 10, 10);
        glColor3f(255 / 255.0, 255 / 255.0, 255 / 255.0); // white
        glTranslatef(0.1, 0.25, 0.05);
        glScalef(1, 1, 1.3);
        glutSolidSphere(0.1, 10, 10);
    };
    glPopMatrix();
}

void Firnen::drawLeg() const {
    // TODO: update the legs
    glPushMatrix();
    {
        glColor3f(128 / 255.0, 64 / 255.0, 0 / 255.0); //
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        glRotatef(30, 1, 0, 0);
        gluCylinder(quadratic, 0.2, 0.7, 1, 20, 2); // top arm
        glTranslatef(0, 0, -0.6);
        gluCylinder(quadratic, 0.15, 0.2, 0.6, 20, 2); // top arm
        glRotatef(-40, 0, 1, 0);
        glTranslatef(0, 0, -0.4);
        gluCylinder(quadratic, 0.2, 0.15, 0.5, 20, 2); // lower arm
        glTranslatef(0, 0, -0.5);
        gluCylinder(quadratic, 0.15, 0.2, 0.5, 20, 2); // lower arm
    };
    glPopMatrix();
}
