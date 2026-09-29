cmake_minimum_required(VERSION 3.20)

project(CppRayTracer
    VERSION 1.0.0
    DESCRIPTION "A modern C++ ray tracer exploring computational geometry, materials and rendering algorithms"
    LANGUAGES CXX
)

# Standard C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for language servers (clangd, vscode)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler Warnings Configuration
add_library(raytracer_warnings INTERFACE)
if(MSVC)
    target_compile_options(raytracer_warnings INTERFACE
        /W4
        /permissive-
    )
else()
    target_compile_options(raytracer_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Third-party: STB Image and Image Write
add_library(stb STATIC
    third_party/stb/stb_impl.cpp
)
target_include_directories(stb SYSTEM PUBLIC
    third_party/stb
)

# Core Ray Tracer Library
add_library(raytracer_core INTERFACE)
target_include_directories(raytracer_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(raytracer_core INTERFACE
    raytracer_warnings
    stb
)

# Main Executable
add_executable(cpp-raytracer
    src/main.cpp
)
target_link_libraries(cpp-raytracer PRIVATE
    raytracer_core
)

# Testing
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
