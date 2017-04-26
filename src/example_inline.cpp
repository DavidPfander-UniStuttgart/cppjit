#include <cstdlib>
#include <iostream>

#include "cppjit/cppjit.hpp"
#include "example_kernels/kernel_inline.hpp"

int main(void) {

  cppjit::init();

  std::string my_kernel_inline_source(
      // "#include \"kernel_impl.hpp\"\n"
      "#include <iostream>\n"
      "extern \"C\" void kernel_impl() {\n"
      "std::cout << \"in inline kernel\" << std::endl;\n"
      "}\n");

  cppjit::detail::compile_inline_kernel(my_kernel_inline_source, "my_inline_kernel");
  cppjit::load_kernel("my_inline_kernel");

  std::cout << "calling kernel (1)..." << std::endl;
  my_kernel_inline();

  std::cout << "calling kernel (2)..." << std::endl;
  my_kernel_inline();

  std::cout << "close kernel..." << std::endl;
  // close_kernel_library();

  cppjit::finalize();

  return 0;
}
