cmake_minimum_required(VERSION 3.15)
project(OpenGL VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)

add_subdirectory(src)
