
#include "WorldObjects/Firnen.hpp"
#include "WorldObjects/FirnensAkkkin.hpp"

FirnensAkkkin m_akkkin;

void Firnen::updateAnimation(double t, double dt) {
    m_akkkin.updateAnimation(t, dt);
    m_armRotation = sin(t * 6.05);
    m_height      = sin(t * 6);
}

void Firnen::update(double t, double dt) {
    WorldObject::update(t, dt);
    updateAnimation(t, dt);
}

/********************************  DRAW FIRNEN ********************************/
// the objects are a simple circle.
void Firnen::draw() const {
    if (!m_visible) {
        return;
    }
    // drawText("Firnen", m_pos, randColor());
    glPushMatrix();
    {
        // move all of firnen to there location
        glTranslated(m_pos.x, m_pos.y, m_pos.z);

        // draw the looking location
        glPushMatrix();
        {
            auto pos = m_arc.cart() * 2;
            glTranslated(pos.x, pos.y, pos.z);
            glutSolidSphere(0.2, 20, 20);
        };
        glPopMatrix();

        // draw their name.
        glPushMatrix();
        {
            std::string name = "Firnen";
            glDisable(GL_LIGHTING);
            glTranslated(-(int)name.size() / 4, 5, 0);
            glScaled(0.01, 0.01, 0.01);
            glColor3d(0.9, 0.2, 0.15);
            for (char c : name) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
            }
            glEnable(GL_LIGHTING);
        };
        glPopMatrix();

        glRotated(-90, 1, 0, 0);
        glScaled(0.7, 0.7, 0.7); // make him smaller
        glScaled(m_radius, m_radius, m_radius);
        // TODO: find out why akkkin isn't loading
        glPushMatrix();
        {
            glTranslated(0, 0, 5 - (m_height * 0.2));
            m_akkkin.draw();
        };
        glPopMatrix();

        glScaled(1, 1, 1 - (m_height * 0.1));
        drawTorso();
        // glutSolidCone( 1, height, 5, 2 );
    };
    glPopMatrix();
}

void Firnen::drawTorso() const {
    glPushMatrix();
    {
        glTranslated(0, 0, 2);
        // body first
        drawBody();
        // left arm
        glPushMatrix();
        {
            glTranslated(0, 1, 0.5);
            glRotated(m_armRotation * 20, 0, 1, 0);
            drawArm();
        };
        glPopMatrix();
        // right arm
        glPushMatrix();
        {
            glScaled(1, -1, 1);
            glTranslated(0, 1, 0.5);
            glRotated(-m_armRotation * 20, 0, 1, 0);
            drawArm();
        };
        glPopMatrix();
        // draw the head
        glTranslated(0, 0, 1.25);
        glScaled(0.8, 0.8, 0.8);
        drawHead();
    };
    glPopMatrix();
}

void Firnen::drawArm() const {
    glPushMatrix();
    {
        glColor3d(219 / 255.0, 150 / 255.0, 105 / 255.0); //
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        glRotated(35, 1, 0, 0);
        gluCylinder(quadratic, 0.2, 0.15, 0.5, 20, 2); // top arm
        glTranslated(0, 0, -0.6);
        gluCylinder(quadratic, 0.15, 0.2, 0.6, 20, 2); // top arm
        glRotated(-40, 0, 1, 0);
        glTranslated(0, 0, -0.4);
        gluCylinder(quadratic, 0.2, 0.15, 0.5, 20, 2); // lower arm
        glTranslated(0, 0, -0.5);
        gluCylinder(quadratic, 0.15, 0.2, 0.5, 20, 2); // lower arm
    };
    glPopMatrix();
}

void Firnen::drawBody() const {
    glPushMatrix();
    {
        glColor3d(153 / 255.0, 153 / 255.0, 45 / 255.0); // nice green tunic
        glScaled(1, 1.2, 1.5);
        glutSolidSphere(1, 10, 10);
    };
    glPopMatrix();
}

void Firnen::drawHead() const {
    // 0, 0 should be the base of the neck
    glPushMatrix();
    {
        glColor3d(219 / 255.0, 150 / 255.0, 105 / 255.0); //
        glPushMatrix();
        {
            GLUquadricObj *quadratic;
            quadratic = gluNewQuadric();
            gluCylinder(quadratic, 0.5, 0.3, 1, 20, 2);
        };
        glPopMatrix();
        glPushMatrix();
        {
            glTranslated(0, 0, 1);
            glutSolidSphere(0.9, 10, 10);
            glPushMatrix();
            {
                glTranslated(0.65, 0.22, 0);
                glRotated(-80, 0, 0, 1);
                drawEye();
            };
            glPopMatrix();
            glPushMatrix();
            {
                glTranslated(0.65, -0.22, 0);
                glRotated(-90, 0, 0, 1);
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
        glTranslated(0.1, 0.25, 0.05);
        glScaled(1, 1, 1.3);
        glutSolidSphere(0.1, 10, 10);
    };
    glPopMatrix();
}

void Firnen::drawLeg() const {
    // TODO: update the legs
    glPushMatrix();
    {
        glColor3d(128 / 255.0, 64 / 255.0, 0 / 255.0); //
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        glRotated(30, 1, 0, 0);
        gluCylinder(quadratic, 0.2, 0.7, 1, 20, 2); // top arm
        glTranslated(0, 0, -0.6);
        gluCylinder(quadratic, 0.15, 0.2, 0.6, 20, 2); // top arm
        glRotated(-40, 0, 1, 0);
        glTranslated(0, 0, -0.4);
        gluCylinder(quadratic, 0.2, 0.15, 0.5, 20, 2); // lower arm
        glTranslated(0, 0, -0.5);
        gluCylinder(quadratic, 0.15, 0.2, 0.5, 20, 2); // lower arm
    };
    glPopMatrix();
}
