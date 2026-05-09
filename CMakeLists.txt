cmake_minimum_required(VERSION 3.15)
project(pine VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(libs/SDL2)

add_subdirectory(src)
