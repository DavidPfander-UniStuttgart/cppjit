#include <cstdlib>
#include <iostream>

#include "cppjit/cppjit.hpp"
#include "example_kernels/kernel_inline.hpp"

CPPJIT_DECLARE_KERNEL(void, test_kernel, (void))

int main(void) {

  cppjit::init();

  // auto f = [](int test) { std::cout << "test: " << test << std::endl; };

  // PRINT_KERNEL_SRC(void kernel_func(int test) { std::cout << "test: " << test
  // << std::endl;});

  // CPPJIT_REGISTER_KERNEL_SOURCE("trivial_kernel", void kernel_func() {
  //   std::cout << "test nicer inlined" << test << std::endl;
  // });

  CPPJIT_REGISTER_KERNEL_SOURCE("test_kernel",
				"#include <iostream> \n extern \"C\" void test_kernel() { std::cout << \"test test kernel\" << std::endl;}");

//     std::string my_kernel_inline_source(
//         "#include <iostream>\n"
//         "extern \"C\" void my_inline_kernel() {\n"
//         "std::cout << \"in inline kernel\" << std::endl;\n"
//         "}\n");

// cppjit::detail::compile_inline_kernel(my_kernel_inline_source,
//                                       "my_inline_kernel");

// std::cout << "calling kernel (1)..." << std::endl;
// my_kernel_inline();

// std::cout << "calling kernel (2)..." << std::endl;
// my_kernel_inline();

// std::cout << "close kernel..." << std::endl;
// // close_kernel_library();

std::cout << "new kernel..." << std::endl;
test_kernel();

cppjit::finalize();

return 0;
}

CPPJIT_DEFINE_KERNEL(void, test_kernel, (void), ())
