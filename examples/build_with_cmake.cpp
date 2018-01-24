#include <cstdlib>
#include <functional>
#include <iostream>

#include <unistd.h>

#include "cppjit/builder/cmake.hpp"
#include "cppjit/cppjit.hpp"

// declares kernel
CPPJIT_DECLARE_KERNEL(bool(int), build_with_cmake_kernel)

int main(void) {

  // only needed to get the absolute path of the current working directory
  std::vector<char> working_directory_buffer(1000);
  char *working_directory = getcwd(working_directory_buffer.data(), 1000);
  if (working_directory == nullptr) {
    std::cout << "error: unable to read working directory" << std::endl;
    return 1;
  }
  std::string absolute_kernel_path;
  absolute_kernel_path += working_directory;
  absolute_kernel_path += "/examples/cmake_kernel";

  // create and setup cmake builder
  std::shared_ptr<cppjit::builder::cmake> builder =
      std::make_shared<cppjit::builder::cmake>("build_with_cmake_kernel");
  cppjit::build_with_cmake_kernel.set_builder(builder);

  // make the compilation and link commands visible on the command line
  builder->set_verbose(true);

  // compiles the source at the source directory using the cmake builder
  cppjit::build_with_cmake_kernel.compile(
      "/home/pfandedd/git/cppjit/examples/cmake_kernel");

  // now use the kernel function
  bool result = cppjit::build_with_cmake_kernel(123);
  std::cout << "result: " << result << std::endl;

  return 0;
}

CPPJIT_DEFINE_KERNEL_NO_SRC(bool(int), build_with_cmake_kernel)
