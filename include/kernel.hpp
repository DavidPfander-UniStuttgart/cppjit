#pragma once

#include <iostream>
#include <vector>

#include <dlfcn.h>

std::vector<void *> opened_kernel_libraries;

void (*kernel_impl)();

void set_kernel_impl() {
  std::cout << "kernel called for first time, loading kernel..." << std::endl;
  void *kernel_library = dlopen("include/libKernel_impl.so", RTLD_LAZY);

  if (!kernel_library) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  dlerror(); /* Clear any existing error */

  *(void **)(&kernel_impl) = dlsym(kernel_library, "kernel_impl");

  char *error = dlerror();
  if (error != nullptr) {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }

  opened_kernel_libraries.push_back(kernel_library);
}

void kernel() {
  if (kernel_impl == nullptr) {
    set_kernel_impl();
  }
  kernel_impl();
}

void cppjit_finalize() {
  for (void *kernel_library : opened_kernel_libraries) {
    dlclose(kernel_library);
  }
  opened_kernel_libraries.clear();
}
