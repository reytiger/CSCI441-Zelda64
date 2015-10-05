#pragma once

// See GitHub for details.
// https://github.com/c42f/tinyformat
#include "tinyformat/tinyformat.h"

enum class Log {
    Info,
    Warning,
    Error,
    Fatal,
};

template <typename... Args>
void log_context(Log loglevel, const char *file, int line, const char *func,
                 const char *fmt, const Args &... args) {
    // Insert a newline here so we catch and indent it below.
    std::string text = "\n" + tfm::format(fmt, args...);
    std::string level;

    // Indent each line with a tab.
    size_t pos = 0;
    while ((pos = text.find('\n', pos)) != std::string::npos) {
        pos += 1; // We want the position *after* the new line.
        text.insert(pos, "\t");
    }

    switch (loglevel) {
    case Log::Info:
        // Info is special.
        tfm::printf("[Info] %s(): %s\n", func, text);
        return;
    case Log::Warning:
        level = "[Warn] ";
        break;
    case Log::Error:
        level = "[Error]";
        break;
    case Log::Fatal:
        level = "[Fatal]";
        break;
    default:
        assert(0 && "Unknown Logging level!");
    }

    tfm::format(std::cerr, "%s %s:%d:%s():%s\n", level, file, line, func, text);
}

#define log_with_context(level, fmt, ...)                                      \
    log_context(level, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__);

#define info(...) log_with_context(Log::Info, ##__VA_ARGS__)
#define warn(...) log_with_context(Log::Warning, ##__VA_ARGS__)
#define error(...) log_with_context(Log::Error, ##__VA_ARGS__)
#define fatal(...)                                                             \
    do {                                                                       \
        log_with_context(Log::Fatal, ##__VA_ARGS__);                           \
        trace();                                                               \
        exit(-1);                                                              \
    } while (0);
