#include <cstdlib>
#include <functional>
#include <iostream>

#include "cppjit/cppjit.hpp"
#include "example_kernels/kernel_inline.hpp"

CPPJIT_DECLARE_KERNEL(int(int), test_kernel)

int main(void) {

  cppjit::init();

  std::cout << "bla" << std::endl;

  cppjit::register_kernel(
      "test_kernel",
      "#include <iostream> \n extern \"C\" int "
      "test_kernel(int input) { std::cout << \"test "
      "test kernel hallo: \" << input << std::endl; return input;}");

  load_test_kernel();
  
  std::cout << "before kernel call" << std::endl;
  int result = test_kernel(8);

  std::cout << "result: " << result << std::endl;

  cppjit::finalize();

  return 0;
}

CPPJIT_DEFINE_KERNEL(int(int), test_kernel)
