#include <cstdlib>
#include <iostream>

#include "cppjit/cppjit.hpp"

// declares kernel
CPPJIT_DECLARE_KERNEL(void(), kernel)

int main(void) {

  // std::cout << "preparing kernel..." << std::endl;
  // set_kernel_impl();

  // std::string kernel_string("");

  set_source_kernel("#include <iostream>\n extern \"C\" void "
                    "test_kernel() { std::cout << \"hello jit world\" << input "
                    "<< std::endl;}");

  std::cout << "calling kernel (1)..." << std::endl;
  kernel();

  std::cout << "calling kernel (2)..." << std::endl;
  kernel();

  std::cout << "close kernel..." << std::endl;
  // close_kernel_library();

  cppjit::finalize();

  return 0;
}

CPPJIT_DEFINE_KERNEL(void(), kernel)
