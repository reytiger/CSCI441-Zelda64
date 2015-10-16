#pragma once
#include "Utils/GL_Defs.hpp"

struct Color {
    union {
        float v[4];
        struct {
            float r;
            float g;
            float b;
            float a;
        };
    };
    Color() : r(), g(), b(), a(1.0f) {}
    Color(double r, double g, double b, double a = 1.0)
        : r(r), g(g), b(b), a(a) {}
    Color(int r, int g, int b, int a = 255)
        : r(r / 255.0), g(g / 255.0), b(b / 255.0), a(a / 255.0) {}
};

inline double getRandd() { return rand() / (double)RAND_MAX; }
inline float getRandf() { return rand() / (float)RAND_MAX; }

inline Color randColor() { return Color(getRandf(), getRandf(), getRandf()); }

inline Color operator*(float num, const Color &color) {
    return Color(num * color.r, num * color.g, num * color.b, color.a);
}

template <typename T>
inline Color operator+(const Color &color, T num) {
    return Color(color.r + num, color.g + num, color.b + num, color.a);
}

template <typename T>
inline Color operator+(T num, const Color &color) {
    return Color(num + color.r, num + color.g, num + color.b, color.a);
}
