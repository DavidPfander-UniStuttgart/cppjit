#include <cstdlib>
#include <functional>
#include <iostream>

#include "cppjit/cppjit.hpp"

// declares kernel
CPPJIT_DECLARE_KERNEL(int(int), test_kernel)

int main(void) {

  std::shared_ptr<cppjit::builder::gcc> builder =
      cppjit::test_kernel.get_builder_as<cppjit::builder::gcc>();

  // make the compilation and link commands visible on the command line
  builder->set_verbose(true);

  // clang command-line interface is compatible to gcc, just change compile and
  // link commands (clang is also ABI compatible)
  builder->set_compiler("clang++");
  builder->set_linker("clang++");

  // setup the kernel source (function generated by declare macro)
  cppjit::test_kernel.compile_inline(
      "#include <iostream> \n extern \"C\" int "
      "test_kernel(int input) { std::cout << \"test "
      "test kernel hallo: \" << input << std::endl; return input;}");

  std::cout << "before kernel call" << std::endl;
  int result = cppjit::test_kernel(8);

  std::cout << "result: " << result << std::endl;

  return 0;
}

CPPJIT_DEFINE_KERNEL(int(int), test_kernel)
