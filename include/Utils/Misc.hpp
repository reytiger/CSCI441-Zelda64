#pragma once

#include <chrono>
#include <functional>

#include "cassert"
#include "cerrno"
#include "cmath"
#include "cstring"

#include "execinfo.h"

// This prints traces as mangled function names. This command makes life easier:
// ./a2 2>&1 | grep a1 | tr '()+' ' ' | awk '{ printf "%s\n", $2; }'
// Then pipe that to c++-filt (which isn't on Alamode!) or use
// http://demangler.com.
#define trace() trace_helper(__FILE__, __LINE__, __FUNCTION__)
#define trace_fatal() (trace(), exit(-1))
void trace_helper(const char *file, int line, const char *func);

#define glChk() check_gl_errors_helper(__FILE__, __LINE__)
void check_gl_errors_helper(const char *file, int line);

void pushMatrixAnd(std::function<void(void)> body);

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

inline double getRand() { return rand() / (double)RAND_MAX; }

inline double now_secs() {
  auto now = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now).count() /
         1000.0;
}

template <typename T, typename U> U lerp(T t, U a, U b) {
  return (1 - t) * a + t * b;
}

template <typename T, typename U> U lerp(T u, U a, U b, T v, U c, U d) {
  return lerp(v, lerp(u, a, b), lerp(u, c, d));
}
