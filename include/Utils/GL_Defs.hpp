#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Utils/DebugTools.hpp"

#include <functional>

static inline void pushMatrixAnd(std::function<void(void)> body) {
    glPushMatrix();
    glChk();

    body();

    glChk();
    glPopMatrix();
    glChk();
}
