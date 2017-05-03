cmake_minimum_required(VERSION 3.2)

project(build_with_cmake_kernel)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -march=native -mtune=native ${CMAKE_CXX_FLAGS}")

file(GLOB SOURCES "build_with_cmake_kernel.cpp")
add_library(build_with_cmake_kernel SHARED ${SOURCES})
