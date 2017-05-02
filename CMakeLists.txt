cmake_minimum_required(VERSION 3.2)

project(cppjit)

option(DISABLE_BIND_FOR_CIRCLE_CI "Circle CI containers don't support binding" OFF)

if (DISABLE_BIND_FOR_CIRCLE_CI)
  MESSAGE(WARNING "HPX is used with BIND=NONE (meant for Circle CI")
  add_definitions(-DDISABLE_BIND_FOR_CIRCLE_CI)
endif()

find_package(PkgConfig REQUIRED)

# find_package(OpenMP REQUIRED)

include_directories(include)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -march=native -mtune=native ${CMAKE_CXX_FLAGS}")

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES_HELLO_WORLD_JIT "examples/hello_world_jit.cpp")
add_executable(hello_world_jit ${SOURCES_HELLO_WORLD_JIT})
target_link_libraries(hello_world_jit dl)

file(GLOB SOURCES_EXAMPLE_INLINE "examples/example_inline_with_explicit_load.cpp")
add_executable(example_inline_with_explicit_load ${SOURCES_EXAMPLE_INLINE})
target_link_libraries(example_inline_with_explicit_load dl)

file(GLOB SOURCES_EXAMPLE_FUNCTIONAL "examples/example_functional.cpp")
add_executable(example_functional ${SOURCES_EXAMPLE_FUNCTIONAL})
target_link_libraries(example_functional dl)

file(GLOB SOURCES_EXAMPLE_CLASS "examples/example_class/example_class.cpp")
add_executable(example_class ${SOURCES_EXAMPLE_CLASS})
target_link_libraries(example_class dl)

file(GLOB SOURCES_TEST "test/test_cppjit.cpp")
add_executable(test_cppjit ${SOURCES_TEST})
target_link_libraries(test_cppjit dl)

