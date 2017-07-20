#include <cstdlib>
#include <iostream>

#include "cppjit/cppjit.hpp"

// declares kernel
CPPJIT_DECLARE_KERNEL(void(), kernel)

int main(void) {

  // set source, compile and load kernel
  cppjit::compile_inline_kernel(
      "#include <iostream>\n extern \"C\" void "
      "kernel() { std::cout << \"hello jit world\" << std::endl;}");

  // call kernel like an other function
  cppjit::kernel();

  return 0;
}

// defines kernel, put in single compilation unit
CPPJIT_DEFINE_KERNEL(void(), kernel)
