#include "Utils.hpp"

#include "execinfo.h"

#define make_case(ERR)                                                         \
  case ERR:                                                                    \
    str = #ERR;                                                                \
    break;

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

void check_opengl(const char *file, int line) {
  const char *str = nullptr;

  int err = GL_NO_ERROR;
  // I think these can stack up.
  while ((err = glGetError()) != GL_NO_ERROR) {
    switch (err) {
      make_case(GL_INVALID_ENUM) make_case(GL_INVALID_VALUE)
          make_case(GL_INVALID_OPERATION)
              make_case(GL_INVALID_FRAMEBUFFER_OPERATION)
                  make_case(GL_OUT_OF_MEMORY) make_case(GL_STACK_UNDERFLOW)
                      make_case(GL_STACK_OVERFLOW)

                          case GL_NO_ERROR : return;

    default:
      // TODO: Print the value.
      str = "Unknown error";
    }

    fprintf(stderr, "%s:%d: %s\n", file, line, str);
    trace();
  }

  exit(1);
}

void check_errno(const char *file, int line) {
  int err = errno;
  if (err) {
    std::cerr << file << ":" << line << ": " << strerror(err) << std::endl;
    trace();
    exit(-1);
  }
}

void check_helper(const char *file, int line) {
  check_errno(file, line);
  check_opengl(file, line);
}
