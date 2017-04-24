#include <iostream>
#include <cstdlib>

#include "kernel.hpp"

int main(void) {

  // std::cout << "preparing kernel..." << std::endl;
  // set_kernel_impl();

  std::string compile_cmd("g++ -o include/kernel_impl.o -c -std=c++14 -fPIC include/kernel_impl.cpp > compile.log 2>&1");
  int return_value = std::system(compile_cmd.c_str());
  if (return_value != 0) {
    std::cerr << "error: compile of kernel failed" << std::endl;
  }
  std::string link_cmd("g++ -shared -o include/libKernel_impl.so include/kernel_impl.o > link.log 2>&1");
  return_value = std::system(link_cmd.c_str());
  if (return_value != 0) {
    std::cerr << "error: compile of kernel failed" << std::endl;
  }

  // std::string kernel_string("");

  std::cout << "calling kernel (1)..." << std::endl;
  kernel();

  std::cout << "calling kernel (2)..." << std::endl;
  kernel();

  std::cout << "close kernel..." << std::endl;
  // close_kernel_library();

  cppjit_finalize();

  return 0;
}
