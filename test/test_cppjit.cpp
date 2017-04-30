#include <cstdlib>
#include <iostream>
#include <string>

#include "cppjit/cppjit.hpp"
#include "cppjit/function_traits.hpp"

namespace test_function_traits {

void test_return_type() {
  std::function<char(int, int, double)> f;
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::return_type,
                   char>::value,
      "function_traits return type does not match");
}

void test_individual_arguments() {
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
  static_assert(std::is_same<cppjit::detail::function_traits<decltype(
                                 function_to_analyse::f)>::return_type,
                             char>::value,
                "function_traits return type does not match");
}

void test_individual_arguments_function() {
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
  std::function<char(int, int, double)> f;
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(
                       function_to_analyse::f)>::args_type,
                   cppjit::detail::pack<int, int, double>>::value,
      "function_traits pack instantiated with all arguments not created");
}

void test_return_type_pointer() {
  char (*f)(int, int, double);
  static_assert(
      std::is_same<cppjit::detail::function_traits<decltype(f)>::return_type,
                   char>::value,
      "function_traits return type does not match");
}

void test_individual_arguments_pointer() {
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

namespace test_call_kernels {

void simple() {

  set_source_simple_kernel("extern \"C\" bool "
                           "simple_kernel(int i, double d) { return "
                           "static_cast<double>(i + 1) == d; "
                           "}");

  if (!simple_kernel(4, 5.0)) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }

  if (simple_kernel(100000, 5.0)) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }
}

void no_arguments() {

  set_source_no_arguments_kernel("extern \"C\" bool "
                                 "no_arguments_kernel(void) { return false; "
                                 "}");

  if (no_arguments_kernel()) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }
}

void test_void() {

  set_source_void_kernel("extern \"C\" void "
                         "void_kernel(void) {}");

  void_kernel();
}

void cpp_arguments() {

  set_source_cpp_arguments_kernel(
      "#include <string> \n extern \"C\" int "
      "cpp_arguments_kernel(std::string s) { return s.size(); }");

  if (cpp_arguments_kernel("12345") != 5) {
    std::cerr << "error: return value did not match: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }
}

void load_kernel() {
  set_source_load_kernel_kernel("extern \"C\" void "
                                "load_kernel_kernel(void) { }");

  if (is_loaded_load_kernel_kernel()) {
    std::cerr << "error: kernel already loaded: " << __FILE__ << " " << __LINE__
              << std::endl;
    exit(1);
  }

  load_load_kernel_kernel();

  if (!is_loaded_load_kernel_kernel()) {
    std::cerr << "error: kernel still not loaded: " << __FILE__ << " "
              << __LINE__ << std::endl;
    exit(1);
  }

  load_kernel_kernel();
}

void test_call_kernels() {
  std::cout << "testing kernel calling" << std::endl;
  simple();
  no_arguments();
  test_void();
  cpp_arguments();
  load_kernel();
}
}

int main(void) {
  std::cout << "info: test exit with an error message if any error occurs"
            << std::endl;
  test_function_traits::test_function_traits();
  test_call_kernels::test_call_kernels();
}
