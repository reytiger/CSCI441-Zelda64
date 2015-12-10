#include "RenderPass.hpp"

void RenderPass::render() {
    glChk();
    if (m_setup) {
        m_setup();
        glChk();
    }

    m_program.use();
    renderQuad();
}

void RenderPass::renderQuad() {
    glDisable(GL_LIGHTING);

    glChk();
    glMatrixMode(GL_PROJECTION);
    pushMatrixAnd([]() {
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glChk();

        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, 1.0, 0.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(0.0, 1.0, 0.0);
        }
        glEnd();
        glChk();

        // Make sure we pop the right matrix.
        glMatrixMode(GL_PROJECTION);
        glChk();
    });
    glChk();
}
