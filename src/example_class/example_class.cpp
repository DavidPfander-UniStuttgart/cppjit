#include "my_class_interface.hpp"

#include "cppjit/cppjit.hpp"

// declares kernel
CPPJIT_DECLARE_KERNEL(my_class_interface *(), my_class_factory_method)

int main(void) {
  set_source_my_class_factory_method(
      "#include \"../../src/example_class/my_class.hpp\" \n extern \"C\" my_class_interface* "
      "my_class_factory_method() { return new my_class(); }");

  my_class_interface *instance = my_class_factory_method();

  bool is_true = instance->f1(5);
  std::cout << "f1(5) -> " << is_true << std::endl;

  is_true = instance->f1(8);
  std::cout << "f1(8) -> " << is_true << std::endl;

  is_true = instance->f2('b');
  std::cout << "f2('b') -> " << is_true << std::endl;

  is_true = instance->f2('z');
  std::cout << "f2('z') -> " << is_true << std::endl;
  
  return 0;
}

CPPJIT_DEFINE_KERNEL(my_class_interface *(), my_class_factory_method)
