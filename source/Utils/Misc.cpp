#include "Utils.hpp"

#include <cassert>
#include <cstdio>

#include "execinfo.h"

void trace_helper(const char *file, int line, const char *func) {
  void *buffer[100] = {};
  size_t size = backtrace(buffer, 100);

  // I'm not really sure when this would happen, so I just give up
  // any chance of a reasonable recovery and shut everything down.
  assert(size > 0);

  fprintf(stderr, "----------%s:%d:%s() Stacktrace----------\n", file, line,
          func);
  backtrace_symbols_fd(buffer + 1, size - 1, 2 /* stderr */);
  fprintf(stderr, "\n");
}

void check_gl_errors_helper(const char *file, int line) {
  GLenum err = glGetError();
  if (err == GL_NO_ERROR) {
    return;
  }

  const char *str = nullptr;

  while (err != GL_NO_ERROR) {

#define make_case(ERR)                                                         \
  case ERR:                                                                    \
    str = #ERR;                                                                \
    break;
    switch (err) {
      make_case(GL_INVALID_ENUM);
      make_case(GL_INVALID_VALUE);
      make_case(GL_INVALID_OPERATION);
      make_case(GL_INVALID_FRAMEBUFFER_OPERATION);
      make_case(GL_OUT_OF_MEMORY);
      make_case(GL_STACK_UNDERFLOW);
      make_case(GL_STACK_OVERFLOW);

    default:
      str = "Unknown error";
    }
#undef make_case

    fprintf(stderr, "%s:%d: %s\n", file, line, str);
    trace();
    err = glGetError();
  }

  exit(1);
}

void pushMatrixAnd(std::function<void(void)> body) {
  glPushMatrix();
  glChk();

  body();

  glChk();
  glPopMatrix();
}
