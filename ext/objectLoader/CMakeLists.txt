cmake_minimum_required(VERSION 3.0)

project("Object Loader")

if (CMAKE_COMPILER_IS_GNUCXX)
    # Disable all warnings. They are of little interest to us.
    set(NO_WARNINGS "${CMAKE_CXX_FLAGS_INIT} -w")
# This *might* catch Clang, but pretend it only catches MSVC for now.
else()
    # Disable all warnings. They are of little interest to us.
    set(NO_WARNINGS "${CMAKE_CXX_FLAGS_INIT} /w")
endif()

file(GLOB OBJLOADER_HEADERS "${S}/ext/objectLoader/*.h")

file(GLOB OBJLOADER_SOURCES "${S}/ext/objectLoader/*.cpp")
list(REMOVE_ITEM OBJLOADER_SOURCES "${S}/ext/objectLoader/main.cpp")

foreach(SOURCE ${OBJLOADER_SOURCES})
        set_source_files_properties(${SOURCE}
        PROPERTIES
            COMPILE_FLAGS "${NO_WARNINGS}")
endforeach()

add_library(objectLoader STATIC
            "${OBJLOADER_SOURCES}"
            "${OBJLOADER_HEADERS}")
