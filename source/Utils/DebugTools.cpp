#include "Utils.hpp"

#ifndef _WIN32
#include "execinfo.h"
#endif

#define make_case(ERR)                                                         \
    case ERR:                                                                  \
        str = #ERR;                                                            \
        break;

void trace_helper(const char *file, int line, const char *func) {
#ifndef _WIN32
    void *buffer[100] = {};
    size_t size       = backtrace(buffer, 100);

    // I'm not really sure when this would happen, so I just give up
    // any chance of a reasonable recovery and shut everything down.
    assert(size > 0 && "backtrace was unexpectedly empty!");

    char **symbols_ptr = backtrace_symbols(buffer, size);
    auto symbols = std::vector<std::string>(symbols_ptr, symbols_ptr + size);
    free(symbols_ptr);

    assert(symbols.size() > 0 && "symbols was unexpectedly empty!");

    std::string tracelist = tfm::format(
        "\n\n----------%s:%d:%s() Stacktrace----------\n", file, line, func);

    int i = 0;
    for (const auto &symbol : symbols) {
        auto line = tfm::format("%3d:\t %s\n", i, symbol);
        tracelist.append(line);
        i -= 1;
    }

    tfm::printf("%s\n", tracelist);
#endif
}

void check_opengl(const char *file, int line) {
    const char *str = nullptr;

    int err = GL_NO_ERROR;
    // I think these can stack up.
    while ((err = glGetError()) != GL_NO_ERROR) {
        switch (err) {
            make_case(GL_INVALID_ENUM);
            make_case(GL_INVALID_VALUE);
            make_case(GL_INVALID_OPERATION);
#ifndef _WIN32
            make_case(GL_INVALID_FRAMEBUFFER_OPERATION);
#endif
            make_case(GL_OUT_OF_MEMORY);
            make_case(GL_STACK_UNDERFLOW);
            make_case(GL_STACK_OVERFLOW);
        case GL_NO_ERROR:
            return;

        default:
            // TODO: Print the value.
            str = "Unknown error";
        }

        error("%s:%d\nOpenGL encountered an error: %s", file, line, str);
        trace();
    }
}

void check_errno(const char *file, int line) {
    int err = errno;
    errno   = 0;

    // This is getting spammed and we have no idea why, so we ignore it.
    if (err == 11) {
        return;
    }
    if (err) {
        error("%s:%d\nSomething set errno: %s", file, line, strerror(err));
        trace();
    }
}

void check_SOIL(const char *file, int line) {
    std::string err = SOIL_last_result();
    if (err == "Image loaded as an OpenGL texture"
        || err == "SOIL initialized") {
        return;
    }
    error("%s:%d\t%s", file, line, err);
    trace();
    abort();
}

static size_t check_helper_count = 0;

void check_helper(const char *file, int line) {
    check_helper_count += 1;
    check_errno(file, line);
    check_opengl(file, line);
    check_SOIL(file, line);

    static const size_t million = 1000000;
    if (check_helper_count % million == 0) {
        info("glChk() called %s million times.", check_helper_count / million);
    }
}
