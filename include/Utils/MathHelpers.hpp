#pragma once

#include <chrono>

#include <cassert>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define PI (as<float>(M_PI))

inline float now_secs() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count()
           / 1000.0f;
}

template <typename T, typename U, typename V>
T clamp(const T &thing, const U &lo, const V &hi) {
    assert(lo <= hi);
    T res = thing;
    if (res < T(lo)) {
        res = T(lo);
    }
    if (res > T(hi)) {
        res = T(hi);
    }
    return res;
}

template <typename T, typename U>
U lerp(T t, U a, U b) {
    return (1 - t) * a + t * b;
}

template <typename T, typename U>
U lerp(T u, U a, U b, T v, U c, U d) {
    return lerp(v, lerp(u, a, b), lerp(u, c, d));
}
