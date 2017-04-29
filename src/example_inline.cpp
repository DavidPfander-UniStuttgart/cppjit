#include <cstdlib>
#include <functional>
#include <iostream>

#include "cppjit/cppjit.hpp"
#include "example_kernels/kernel_inline.hpp"

// CPPJIT_DECLARE_KERNEL(void, test_kernel, (int input))

CPPJIT_DECLARE_KERNEL_NEW(int(int), test_kernel)

// template <typename ret, typename... Ts> ret test_kernel(Ts... args) {
//   if (!cppjit::kernels::test_kernel) {
//     std::cout << "help! not initialized" << std::endl;
//     cppjit::detail::compile_inline_kernel(
//         cppjit::detail::kernel_source_map["test_kernel"], "test_kernel");
//     void *uncasted_function = cppjit::load_kernel("test_kernel");
//     ret (*fp)(Ts...) = reinterpret_cast<decltype(fp)>(uncasted_function);
//     cppjit::kernels::test_kernel = fp;
//     // std::function<int(int)> f =
//     reinterpret_cast<int*(int)*>(uncasted_function);
//   }
//   return cppjit::kernels::test_kernel(std::forward<Ts>(args)...);
// }

// namespace cppjit {
// namespace kernels {
// std::function<int(int)> test_load_me = [](int i) { return i; };
// std::function<int(int)> test;
// }
// }

// template <typename ret, typename... Ts> ret test(Ts... args) {
//   if (!cppjit::kernels::test) {
//     std::cout << "help! not initialized" << std::endl;
//     cppjit::kernels::test = cppjit::kernels::test_load_me;
//   }
//   return cppjit::kernels::test(std::forward<Ts>(args)...);
// }

int main(void) {

  cppjit::init();

  // int return_value = test<int>(8);
  // std::cout << "returned value is: " << return_value << std::endl;

  // std::function<void(int)> my_f;

  // if (!my_f) {
  //   std::cout << "my_f not initialized" << std::endl;
  // }

  // // my_f = [](int) -> std::function<void(int)>::result_type {
  // my_f = [](int) -> decltype(my_f(2)) {
  //   std::cout << "hello" << std::endl;
  // };

  // my_f(8);

  // // auto f = [](int test) { std::cout << "test: " << test << std::endl; };

  // // PRINT_KERNEL_SRC(void kernel_func(int test) { std::cout << "test: " <<
  // test
  // // << std::endl;});

  // // CPPJIT_REGISTER_KERNEL_SOURCE("trivial_kernel", void kernel_func() {
  // //   std::cout << "test nicer inlined" << test << std::endl;
  // // });

  std::cout << "bla" << std::endl;

  CPPJIT_REGISTER_KERNEL_SOURCE(
      "test_kernel",
      "#include <iostream> \n extern \"C\" int "
      "test_kernel(int input) { std::cout << \"test "
      "test kernel hallo: \" << input << std::endl; return input;}");

  int result = test_kernel<int>(8);

  std::cout << "result: " << result << std::endl;

  // //     std::string my_kernel_inline_source(
  // //         "#include <iostream>\n"
  // //         "extern \"C\" void my_inline_kernel() {\n"
  // //         "std::cout << \"in inline kernel\" << std::endl;\n"
  // //         "}\n");

  // // cppjit::detail::compile_inline_kernel(my_kernel_inline_source,
  // //                                       "my_inline_kernel");

  // // std::cout << "calling kernel (1)..." << std::endl;
  // // my_kernel_inline();

  // // std::cout << "calling kernel (2)..." << std::endl;
  // // my_kernel_inline();

  // // std::cout << "close kernel..." << std::endl;
  // // // close_kernel_library();

  // std::cout << "new kernel..." << std::endl;

  // std::function<void(int)> f = test_kernel;

  // test_kernel(3);
  // test_kernel(7);
  // test_kernel(15);

  // std::cout << "calling f(4): " << std::endl;
  // f(4);

  cppjit::finalize();

  return 0;
}

// CPPJIT_DEFINE_KERNEL(void, test_kernel, (int input), (input))

CPPJIT_DEFINE_KERNEL_NEW(int(int), test_kernel)
