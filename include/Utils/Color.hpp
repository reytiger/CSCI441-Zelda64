#pragma once
#include "Utils/GL_Defs.hpp"

struct Color {
  float r;
  float g;
  float b;

  Color() : r(), g(), b() {}
  Color(float r, float g, float b) : r(r), g(g), b(b) {}

  void glSet() { glColor3f(r, g, b); }
};

inline double getRand() { return rand() / (double)RAND_MAX; }

inline Color rand_color() { return Color(getRand(), getRand(), getRand()); }
