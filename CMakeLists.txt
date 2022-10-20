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
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.15.0)

project(${PROJECT_NAME})

# Use wildcard to include all source files under /src
# Can also use GLOB_RECURSE
file(GLOB SOURCES "src/*.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(BINDINGS
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_sdl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_sdl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_sdlrenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_sdlrenderer.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES} ${BINDINGS})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
