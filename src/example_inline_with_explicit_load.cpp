#include <cstdlib>
#include <functional>
#include <iostream>

#include "cppjit/cppjit.hpp"
#include "example_kernels/kernel_inline.hpp"

// declares kernel
CPPJIT_DECLARE_KERNEL(int(int), test_kernel)

int main(void) {

  cppjit::init();

  set_source_test_kernel(
      "#include <iostream> \n extern \"C\" int "
      "test_kernel(int input) { std::cout << \"test "
      "test kernel hallo: \" << input << std::endl; return input;}");

  std::cout << "doing explicit load" << std::endl;
  load_test_kernel();

  std::cout << "before kernel call" << std::endl;
  int result = test_kernel(8);

  std::cout << "result: " << result << std::endl;

  cppjit::finalize();

  return 0;
}

CPPJIT_DEFINE_KERNEL(int(int), test_kernel)
