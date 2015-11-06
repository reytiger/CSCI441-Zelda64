#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "SOIL/SOIL.h"

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

// Lazyman's static_cast. Now with 88% fewer characters!
template <typename T, typename U>
T as(const U &other) {
    return static_cast<T>(other);
}
