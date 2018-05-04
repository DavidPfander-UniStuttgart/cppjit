cmake_minimum_required(VERSION 3.5)

project(cppjit)
set(cppjit_VERSION 0.1.0)

add_library(cppjit INTERFACE)
target_include_directories(cppjit INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
  $<INSTALL_INTERFACE:include/>
  )
target_link_libraries(cppjit INTERFACE dl)

install(TARGETS cppjit EXPORT cppjitTargets)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cppjitConfigVersion.cmake"
  VERSION ${cppjit_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

export(EXPORT cppjitTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cppjitTargets.cmake"
  NAMESPACE cppjit::
  )

configure_file(cppjitConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/cppjitConfig.cmake"
  COPYONLY
  )
set(ConfigPackageLocation lib/cmake/cppjit)

install(EXPORT cppjitTargets
  FILE
  cppjitTargets.cmake
  NAMESPACE
  cppjit::
  DESTINATION
  ${ConfigPackageLocation}
  )

install(
  FILES
  cppjitConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/cppjitConfigVersion.cmake"
  DESTINATION
  ${ConfigPackageLocation}
  COMPONENT
  Devel
  )

# install headers and preserve directory structure
INSTALL (
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h*")

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -march=native -mtune=native ${CMAKE_CXX_FLAGS}")

set(SOURCES_HELLO_WORLD_JIT "examples/hello_world_jit.cpp")
add_executable(hello_world_jit ${SOURCES_HELLO_WORLD_JIT})
target_link_libraries(hello_world_jit cppjit)

set(SOURCES_JIT_WITH_CLANG "examples/jit_with_clang.cpp")
add_executable(jit_with_clang ${SOURCES_JIT_WITH_CLANG})
target_link_libraries(jit_with_clang cppjit)

set(SOURCES_EXAMPLE_FUNCTIONAL "examples/example_functional.cpp")
add_executable(example_functional ${SOURCES_EXAMPLE_FUNCTIONAL})
target_link_libraries(example_functional cppjit)

set(SOURCES_EXAMPLE_CLASS "examples/example_class/example_class.cpp")
add_executable(example_class ${SOURCES_EXAMPLE_CLASS})
target_link_libraries(example_class cppjit)

set(SOURCES_BUILD_WITH_CMAKE "examples/build_with_cmake.cpp")
add_executable(build_with_cmake ${SOURCES_BUILD_WITH_CMAKE})
target_link_libraries(build_with_cmake cppjit)

set(SOURCES_STANDARD_CPP_FILE_KERNEL "examples/standard_cpp_file_kernel.cpp")
add_executable(standard_cpp_file_kernel ${SOURCES_STANDARD_CPP_FILE_KERNEL})
target_link_libraries(standard_cpp_file_kernel cppjit)

set(SOURCES_TEST "test/test_cppjit.cpp")
add_executable(test_cppjit ${SOURCES_TEST})
target_link_libraries(test_cppjit cppjit)

install(TARGETS hello_world_jit jit_with_clang example_functional example_class build_with_cmake test_cppjit RUNTIME DESTINATION bin)

