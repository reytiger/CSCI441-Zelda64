cmake_minimum_required(VERSION 2.8)

project("Object Loader")

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    # Disable all warnings. They are of little interest to us.
    set(NO_WARNINGS "${CMAKE_CXX_FLAGS_INIT} -w")
# This *might* catch Clang, but pretend it only catches MSVC for now.
else()
    # Disable all warnings. They are of little interest to us.
    set(NO_WARNINGS "${CMAKE_CXX_FLAGS_INIT} /w")
endif()

file(GLOB OBJLOADER_HEADERS "${S}/ext/modelLoader/*.h")

file(GLOB OBJLOADER_SOURCES "${S}/ext/modelLoader/*.cpp")
list(REMOVE_ITEM OBJLOADER_SOURCES "${S}/ext/modelLoader/main.cpp")

foreach(SOURCE ${OBJLOADER_SOURCES})
        set_source_files_properties(${SOURCE}
        PROPERTIES
            COMPILE_FLAGS "${NO_WARNINGS}")
endforeach()

add_library(modelLoader STATIC
            ${OBJLOADER_SOURCES}
            ${OBJLOADER_HEADERS})
