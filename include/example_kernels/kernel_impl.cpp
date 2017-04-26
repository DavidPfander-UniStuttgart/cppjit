#include <iostream>

extern "C" void kernel_impl() { std::cout << "in kernel" << std::endl; }
