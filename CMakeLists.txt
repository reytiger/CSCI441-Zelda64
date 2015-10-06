cmake_minimum_required(VERSION 2.8)
project(GuildWars)

option(AUTO_FORMAT "Use clang-format to automatically format code before building" OFF)

set(S "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${S}/cmake")

find_package(OpenGL)
include(FindFreeGLUT)

find_package(OpenAL)
include(FindALUT)

# CMake must be manually updated anytime a new file is added, when using globs.
# You can
#       (1) update this file's timestamp, or
#       (2) re-run 'cmake' manually
file(GLOB_RECURSE sources "${S}/source/*.cpp")
file(GLOB_RECURSE includes "${S}/include/*.hpp")

include_directories("${FREEGLUT_INCLUDE_DIR}")
include_directories("${S}/include")

if (CMAKE_COMPILER_IS_GNUCXX)
  # Misc flags
  set(flags "-std=c++11 -fmax-errors=5 -fdiagnostics-color")

  # Warnings to turn ON
  set(Wwarnings "-Wall -Wextra")

  # Warnings to turn OFF
  set(Wno-warnings "")

# This *might* catch Clang, but pretend it only catches MSVC for now.
else()
  # Misc flags
  set(flags "")

  # Warnings to turn ON
  set(Wwarnings "/W3")

  # Warnings to turn OFF
  set(Wno-warnings "")

  # MSVC provides safe versions of a ton of functions, and then warns if you don't
  # use them.
  # We're not using them, so disable the warnings.
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Wwarnings} ${Wno-warnings} ${flags}")

add_executable(guildWars ${sources} ${includes})

target_link_libraries(guildWars
                      ${OPENGL_LIBRARIES}
                      ${FREEGLUT_LIBRARIES}
                      ${OPENAL_LIBRARIES}
                      ${ALUT_LIBRARIES})

if("${AUTO_FORMAT}")
  add_custom_target(formatting COMMAND clang-format ${sources} ${includes} -i)
  add_dependencies(guildWars formatting)
endif()


