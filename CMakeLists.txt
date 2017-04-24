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
file(GLOB SOURCES_COMMON
  "src/example.cpp"
)

set(SOURCES_CPPJIT ${SOURCES_COMMON})

add_executable(example ${SOURCES_CPPJIT})
target_link_libraries(example dl)
# INSTALL_TARGETS(/bin example)
