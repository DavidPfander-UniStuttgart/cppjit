#include <iostream>

extern "C" void cpp_kernel() {
  std::cout << "hello from JIT-compiled cpp file!" << std::endl;
}
