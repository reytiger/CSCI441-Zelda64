#pragma once

// Windows doesn't define M_PI in cmath without coaxing.
// We need M_PI, so coax it.
#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

// All of the Utils headers.
#include "Utils/Color.hpp"
#include "Utils/DebugTools.hpp"
#include "Utils/GL_Defs.hpp"
#include "Utils/MathHelpers.hpp"
#include "Utils/PointVecBase.hpp"
#include "Utils/Logging.hpp"

// Common includes
#include <memory>  // std::unique_ptr, std::make_unique
#include <utility> // std::pair

#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstring>

// ... I've given up.
void drawText(const std::string &text, Vec pos, Color color);
