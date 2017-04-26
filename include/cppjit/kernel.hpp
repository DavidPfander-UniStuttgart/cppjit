#pragma once

#include <iostream>
#include <vector>

#include "cppjit.hpp"

#include <dlfcn.h>

// std::vector<void *> opened_kernel_libraries;

namespace cppjit {
namespace kernels {

void (*kernel_impl)();

void load_kernel_impl() {
  std::cout << "kernel called for first time, loading kernel..." << std::endl;
    void *uncasted_function = cppjit::load_kernel("kernel_impl");
  *(void **)(&kernel_impl) = uncasted_function;
  
}
}
}

void kernel() {
  if (cppjit::kernels::kernel_impl == nullptr) {
    cppjit::detail::compile_kernel("include/", "kernel_impl");
    cppjit::kernels::load_kernel_impl();
  }
  cppjit::kernels::kernel_impl();
}
