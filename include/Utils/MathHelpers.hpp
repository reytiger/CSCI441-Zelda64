#pragma once

#include <chrono>

#include <cassert>

inline double now_secs() {
  auto now = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now).count() /
         1000.0;
}

template <typename T> T clamp(const T &thing, const T &lo, const T &hi) {
  assert(lo <= hi);
  T res = thing;
  if (res < lo) {
    res = lo;
  }
  if (res > hi) {
    res = hi;
  }
  return res;
}

template <typename T, typename U> U lerp(T t, U a, U b) {
  return (1 - t) * a + t * b;
}

template <typename T, typename U> U lerp(T u, U a, U b, T v, U c, U d) {
  return lerp(v, lerp(u, a, b), lerp(u, c, d));
}
