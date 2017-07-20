#include <cstdlib>
#include <iostream>
#include <string>

#include "cppjit/cppjit.hpp"
#include "cppjit/function_traits.hpp"

namespace test_function_traits {

void test_return_type() {
  std::cout
      << "testing function traits return type member, std::function version "
         "(compile-time check)"
      << std::endl;
  std::function<char(int, int, double)> f;
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::return_type,
                   char>::value,
      "function_traits return type does not match");
}

void test_individual_arguments() {
  std::cout << "testing function traits individual arguement member, "
               "std::function version "
               "(compile-time check)"
            << std::endl;
  std::function<char(int, int, double)> f;
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::arg_type<0>,
                   int>::value,
      "function_traits first argument type did not match");

  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::arg_type<1>,
                   int>::value,
      "function_traits second argument type did not match");

  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::arg_type<2>,
                   double>::value,
      "function_traits third argument type did not match");
}

void test_all_arguments() {
  std::cout
      << "testing function traits all arguments member, std::function version "
         "(compile-time check)"
      << std::endl;
  std::function<char(int, int, double)> f;
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::args_type,
                   cppjit::detail::pack<int, int, double>>::value,
      "function_traits pack instantiated with all arguments not created");
}

namespace function_to_analyse {
char f(int, int, double) { return 'c'; }
}

void test_return_type_function() {
  std::cout << "testing function traits return type member, function version "
               "(compile-time check)"
            << std::endl;
  static_assert(std::is_same<cppjit::detail::function_traits<decltype(
                                 function_to_analyse::f)>::return_type,
                             char>::value,
                "function_traits return type does not match");
}

void test_individual_arguments_function() {
  std::cout << "testing function traits individual arguement member, "
               "function version "
               "(compile-time check)"
            << std::endl;
  static_assert(std::is_same<cppjit::detail::function_traits<decltype(
                                 function_to_analyse::f)>::arg_type<0>,
                             int>::value,
                "function_traits first argument type did not match");

  static_assert(std::is_same<cppjit::detail::function_traits<decltype(
                                 function_to_analyse::f)>::arg_type<1>,
                             int>::value,
                "function_traits second argument type did not match");

  static_assert(std::is_same<cppjit::detail::function_traits<decltype(
                                 function_to_analyse::f)>::arg_type<2>,
                             double>::value,
                "function_traits third argument type did not match");
}

void test_all_arguments_function() {
  std::cout << "testing function traits all arguments member, function version "
               "(compile-time check)"
            << std::endl;
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(
                       function_to_analyse::f)>::args_type,
                   cppjit::detail::pack<int, int, double>>::value,
      "function_traits pack instantiated with all arguments not created");
}

void test_return_type_pointer() {
  std::cout << "testing function traits return type member, pointer version "
               "(compile-time check)"
            << std::endl;
  char (*f)(int, int, double);
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::return_type,
                   char>::value,
      "function_traits return type does not match");
}

void test_individual_arguments_pointer() {
  std::cout
      << "testing function traits individual arguement member, pointer version "
         "(compile-time check)"
      << std::endl;
  char (*f)(int, int, double);
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::arg_type<0>,
                   int>::value,
      "function_traits first argument type did not match");

  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::arg_type<1>,
                   int>::value,
      "function_traits second argument type did not match");

  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::arg_type<2>,
                   double>::value,
      "function_traits third argument type did not match");
}

void test_all_arguments_pointer() {
  std::cout << "testing function traits all arguments member, pointer version "
               "(compile-time check)"
            << std::endl;
  char (*f)(int, int, double);
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::args_type,
                   cppjit::detail::pack<int, int, double>>::value,
      "function_traits pack instantiated with all arguments not created");
}

void test_function_traits() {
  std::cout << "testing function_pointer_traits" << std::endl;
  test_return_type();
  test_individual_arguments();
  test_all_arguments();

  test_return_type_function();
  test_individual_arguments_function();
  test_all_arguments_function();

  test_return_type_pointer();
  test_individual_arguments_pointer();
  test_all_arguments_pointer();
}
}

CPPJIT_DECLARE_KERNEL(bool(int, double), simple_kernel)
CPPJIT_DEFINE_KERNEL(bool(int, double), simple_kernel)

CPPJIT_DECLARE_KERNEL(bool(void), no_arguments_kernel)
CPPJIT_DEFINE_KERNEL(bool(void), no_arguments_kernel)

CPPJIT_DECLARE_KERNEL(void(void), void_kernel)
CPPJIT_DEFINE_KERNEL(void(void), void_kernel)

CPPJIT_DECLARE_KERNEL(int(std::string), cpp_arguments_kernel)
CPPJIT_DEFINE_KERNEL(int(std::string), cpp_arguments_kernel)

CPPJIT_DECLARE_KERNEL(void(void), load_kernel_kernel)
CPPJIT_DEFINE_KERNEL(void(void), load_kernel_kernel)

CPPJIT_DECLARE_KERNEL(void(void), kernel_with_errors_kernel)
CPPJIT_DEFINE_KERNEL(void(void), kernel_with_errors_kernel)

namespace test_call_kernels {

void simple() {
  std::cout << "testing base case" << std::endl;
  cppjit::get_builder_simple_kernel()->set_verbose(true);
  cppjit::compile_inline_simple_kernel(
      "extern \"C\" bool "
      "simple_kernel(int i, double d) { return "
      "static_cast<double>(i + 1) == d; "
      "}");

  if (!cppjit::simple_kernel(4, 5.0)) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }

  if (cppjit::simple_kernel(100000, 5.0)) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }
}

void no_arguments() {
  std::cout << "testing kernel without arguments" << std::endl;
  cppjit::get_builder_no_arguments_kernel()->set_verbose(true);
  cppjit::compile_inline_no_arguments_kernel(
      "extern \"C\" bool "
      "no_arguments_kernel(void) { return false; "
      "}");

  if (cppjit::no_arguments_kernel()) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }
}

void test_void() {
  std::cout << "testing kernel with void arguments" << std::endl;
  cppjit::get_builder_void_kernel()->set_verbose(true);
  cppjit::compile_inline_void_kernel("extern \"C\" void "
                                     "void_kernel(void) {}");

  cppjit::void_kernel();
}

void cpp_arguments() {
  std::cout << "testing kernel with c++ arguments" << std::endl;
  cppjit::get_builder_cpp_arguments_kernel()->set_verbose(true);
  cppjit::compile_inline_cpp_arguments_kernel(
      "#include <string> \n extern \"C\" int "
      "cpp_arguments_kernel(std::string s) { return s.size(); }");

  if (cppjit::cpp_arguments_kernel("12345") != 5) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }
}

void load_kernel() {
  std::cout << "testing manual loading of kernels" << std::endl;
  cppjit::get_builder_load_kernel_kernel()->set_verbose(true);

  if (cppjit::is_compiled_load_kernel_kernel()) {
    std::cerr << "error: kernel already loaded: " << __FILE__ << " " << __LINE__
              << std::endl;
    exit(1);
  }

  cppjit::compile_inline_load_kernel_kernel("extern \"C\" void "
                                            "load_kernel_kernel(void) { }");

  if (!cppjit::is_compiled_load_kernel_kernel()) {
    std::cerr << "error: kernel still not loaded: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }

  cppjit::load_kernel_kernel();
}

void kernel_with_errors() {
  std::cout << "testing error thrown on kernel with errors" << std::endl;
  cppjit::get_builder_kernel_with_errors_kernel()->set_verbose(true);

  try {
    cppjit::compile_inline_kernel_with_errors_kernel(
        "extern \"C\" void "
        "kernel_with_errors_kernel() { this is not a valid symbol; }");
  } catch (cppjit::cppjit_exception &e) {
    return;
  }
  std::cerr << "error: did not throw exception: " << __FILE__ << " " << __LINE__
            << std::endl;
  exit(1);
}

void test_call_kernels() {
  std::cout << "testing kernel calling" << std::endl;
  simple();
  no_arguments();
  test_void();
  cpp_arguments();
  load_kernel();
  kernel_with_errors();
}
}

namespace test_helpers {

void other_kernel_directory() {
  std::shared_ptr<cppjit::builder::builder> builder =
      cppjit::get_builder_void_kernel();
  builder->set_verbose(true);
  std::string old_kernel_tmp_dir = builder->get_compile_dir();
  builder->set_compile_dir("./kernels_tmp/cppjit_test_tmp_folder", true);

  // reset source for void kernel to trigger (re)build
  try {
    cppjit::compile_inline_void_kernel("extern \"C\" void "
                               "void_kernel(void) {}");
    cppjit::void_kernel();
  } catch (cppjit::cppjit_exception &e) {
    std::cerr << "error: changing the kernel directory lead to non-functional "
                 "kernel: "
              << __FILE__ << " " << __LINE__ << std::endl;
    exit(1);
  }
}

void test_helpers() { other_kernel_directory(); }
}

int main(void) {
  // cppjit::init();
  // cppjit::set_verbose(true);
  std::cout << "info: test exits with an error message if any error occurs"
            << std::endl;
  test_function_traits::test_function_traits();
  test_call_kernels::test_call_kernels();
  test_helpers::test_helpers();
  // cppjit::finalize();
  std::cout << "no early exit, tests ran successfully" << std::endl;
}
