#include <iostream>
#include <cstdlib>

#include "cppjit/cppjit.hpp"
#include "example_kernels/kernel.hpp"

int main(void) {

  cppjit::init();

  // std::cout << "preparing kernel..." << std::endl;
  // set_kernel_impl();

  // std::string kernel_string("");

  std::cout << "calling kernel (1)..." << std::endl;
  kernel();

  std::cout << "calling kernel (2)..." << std::endl;
  kernel();

  std::cout << "close kernel..." << std::endl;
  // close_kernel_library();

  cppjit::finalize();

  return 0;
}
