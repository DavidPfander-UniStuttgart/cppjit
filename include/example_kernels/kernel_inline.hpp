#pragma once

#include <iostream>
#include <vector>

#include "cppjit/cppjit.hpp"

#include <dlfcn.h>

// std::vector<void *> opened_kernel_libraries;

namespace cppjit {
namespace kernels {

void (*kernel_my_inline_kernel_impl)();
}
}

void my_kernel_inline() {
  if (cppjit::kernels::kernel_my_inline_kernel_impl == nullptr) {
    // cppjit::detail::compile_inline_kernel(my_kernel_inline_source,
    //                                   "my_inline_kernel");
    std::cout << "kernel called for first time, loading kernel..." << std::endl;
    void *uncasted_function = cppjit::load_kernel("my_inline_kernel");
    *(void **)(&cppjit::kernels::kernel_my_inline_kernel_impl) =
        uncasted_function;
  }
  cppjit::kernels::kernel_my_inline_kernel_impl();
}
