#include <cstdlib>
#include <iostream>

#include "cppjit/cppjit.hpp"

// declares and defines kernel
CPPJIT_KERNEL(void(), cpp_kernel, "examples/cpp_kernel")

int main(void) {

  cppjit::cpp_kernel.set_verbose(true);
  // call kernel like an other function
  cppjit::cpp_kernel();

  return 0;
}
