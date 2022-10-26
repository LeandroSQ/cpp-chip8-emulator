set(PROJECT_NAME chip8)

# https://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.15.0)

project(${PROJECT_NAME})

# Use wildcard to include all source files under /src
# Can also use GLOB_RECURSE
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(BINDINGS
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_sdl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_sdl.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES} ${BINDINGS})

# Adds the precompiled header iostream to the target
target_precompile_headers(${PROJECT_NAME} PUBLIC <iostream> <cstdint> <functional> <string> <vector> <sstream> <fstream> <filesystem> <SDL2/SDL.h> <imgui.h> "bindings/imgui_impl_sdl.h" <implot.h> <implot_internal.h>)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

# Copies the ROMs to the build directory
file(COPY roms DESTINATION ${CMAKE_BINARY_DIR}/bin)