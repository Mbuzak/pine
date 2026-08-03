cmake_minimum_required(VERSION 3.15)
project(pine VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(src)

add_subdirectory(libs/SDL2)
add_subdirectory(libs/cglm/)
