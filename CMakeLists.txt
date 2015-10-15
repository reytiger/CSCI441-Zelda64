cmake_minimum_required(VERSION 2.8)
project(GuildWars)

option(AUTO_FORMAT "Use clang-format to automatically format code before building" OFF)

set(S "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${S}/cmake")

find_package(OpenGL)
include(FindFreeGLUT)

find_package(OpenAL)
include(FindALUT)

# tinyformat
include_directories("${S}/ext/")

# yaml stuff
include_directories("${S}/ext/yaml/include")

set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_BUILD_CONTRIB OFF)
add_subdirectory("${S}/ext/yaml")


# CMake must be manually updated anytime a new file is added, when using globs.
# You can
#       (1) update this file's timestamp, or
#       (2) re-run 'cmake' manually
# We do it this way instead of recurisve globs to preserve the file structure in
# projects like Visual Studio.
file(GLOB sources              "${S}/source/*.cpp")
file(GLOB sources_cameras      "${S}/source/Cameras/*.cpp")
file(GLOB sources_utils        "${S}/source/Utils/*.cpp")
file(GLOB sources_worldobjects "${S}/source/WorldObjects/*.cpp")
file(GLOB sources_lighting     "${S}/source/WorldObjects/Lighting/*.cpp")

file(GLOB includes              "${S}/include/*.hpp")
file(GLOB includes_cameras      "${S}/include/Cameras/*.hpp")
file(GLOB includes_utils        "${S}/include/Utils/*.hpp")
file(GLOB includes_worldobjects "${S}/include/WorldObjects/*.hpp")
file(GLOB includes_lighting     "${S}/include/WorldObjects/Lighting/*.hpp")

list(APPEND sources ${sources_cameras})
list(APPEND sources ${sources_utils})
list(APPEND sources ${sources_worldobjects})
list(APPEND sources ${sources_lighting})

list(APPEND includes ${includes_cameras})
list(APPEND includes ${includes_utils})
list(APPEND includes ${includes_worldobjects})
list(APPEND includes ${includes_lighting})

source_group("Header Files\\Cameras"      FILES ${includes_cameras})
source_group("Header Files\\Utils"        FILES ${includes_utils})
source_group("Header Files\\WorldObjects" FILES ${includes_worldobjects})
source_group("Header Files\\WorldObjects\\Lighting" FILES ${includes_lighting})

source_group("Source Files\\Cameras"      FILES ${sources_cameras})
source_group("Source Files\\Utils"        FILES ${sources_utils})
source_group("Source Files\\WorldObjects" FILES ${sources_worldobjects})

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
                      ${ALUT_LIBRARIES}
                      yaml-cpp)

if("${AUTO_FORMAT}")
  add_custom_target(formatting COMMAND clang-format ${sources} ${includes} -i)
  add_dependencies(guildWars formatting)
endif()
