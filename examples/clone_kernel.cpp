#include <cstdlib>
#include <iostream>

#include "cppjit/cppjit.hpp"

// declares and defines kernel
CPPJIT_KERNEL_NO_SRC(void(), my_kernel)

int main(void) {

  // set source, compile and load kernel
  cppjit::my_kernel.set_verbose(true);
  cppjit::my_kernel.compile_inline(
      "#include <iostream>\n extern \"C\" void "
      "my_kernel() { std::cout << \"hello jit world\" << std::endl;}");

  auto kernel_copy(cppjit::my_kernel);
  cppjit::my_kernel();

  kernel_copy.set_verbose(true);
  kernel_copy();

  return 0;
}
