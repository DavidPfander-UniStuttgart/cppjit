#include <functional>
#include <iostream>

#include "cppjit/cppjit.hpp"
#include "cppjit/function_traits.hpp"

int f(char) { return 7; }

CPPJIT_DECLARE_KERNEL(int(int), wrapper)

int main(void) {
  std::cout << "functional example" << std::endl;

  std::function<int(char)> f_functional;
  std::function<int(char, int)> f_functional2;

  std::cout
      << typeid(
             cppjit::detail::function_pointer_traits<decltype(f)>::return_type)
             .name()
      << std::endl;

  std::cout << "return_type: "
            << typeid(cppjit::detail::function_pointer_traits<decltype(
                          f_functional)>::return_type)
                   .name()
            << std::endl;
  std::cout
      << "arity:"
      << cppjit::detail::function_pointer_traits<decltype(f_functional)>::arity
      << std::endl;
  std::cout
      << "arity:"
      << cppjit::detail::function_pointer_traits<decltype(f_functional2)>::arity
      << std::endl;
  std::cout << "second arg type:"
            << typeid(cppjit::detail::function_pointer_traits<decltype(
                          f_functional2)>::arg_type<1>)
                   .name()
            << std::endl;

  set_source_wrapper(
      "#include <iostream> \n extern \"C\" int "
      "wrapper(int input) { std::cout << \"test "
      "hallo wrapper!: \" << input << std::endl; return input;}");

  int arg = 18;
  wrapper(arg);

  return 0;
}

CPPJIT_DEFINE_KERNEL(int(int), wrapper)
