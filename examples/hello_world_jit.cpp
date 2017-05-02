#include <cstdlib>
#include <iostream>

#include "cppjit/cppjit.hpp"

// declares kernel
CPPJIT_DECLARE_KERNEL(void(), kernel)

int main(void) {

  // cppjit::init();
  
  // // optional: prints additional status messages to stdout
  // cppjit::set_verbose(true);

  compile_inline_kernel(
      "#include <iostream>\n extern \"C\" void "
      "kernel() { std::cout << \"hello jit world\" << std::endl;}");

  // call kernel like an other function
  kernel();

  // optional:: closes loaded libraries
  // cppjit::finalize();

  return 0;
}

// defines kernel
CPPJIT_DEFINE_KERNEL(void(), kernel)
