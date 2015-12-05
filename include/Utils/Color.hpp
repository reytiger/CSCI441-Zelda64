#pragma once
#include "Utils/GL_Defs.hpp"

#include <cassert>

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
        : r(as<float>(r)), g(as<float>(g)), b(as<float>(b)), a(as<float>(a)) {}
    Color(int r, int g, int b, int a = 255)
        : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}
};

inline float getRand() { return rand() / (float)RAND_MAX; }
inline float getRand(float max) { return max * getRand(); }
inline float getRand(float min, float max) {
    assert(max > min);
    return getRand(max - min) + min;
}

inline Color randColor() { return Color(getRand(), getRand(), getRand()); }

template <typename Num>
inline Color operator*(Num num, const Color &color) {
    return Color(num * color.r, num * color.g, num * color.b, color.a);
}

template <typename Num>
inline Color operator+(const Color &color, Num num) {
    return Color(color.r + num, color.g + num, color.b + num, color.a);
}

template <typename Num>
inline Color operator+(Num num, const Color &color) {
    return Color(num + color.r, num + color.g, num + color.b, color.a);
}
