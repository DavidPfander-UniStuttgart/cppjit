#pragma once

#include <fstream>
#include <vector>
#include <map>

#include <dlfcn.h>

namespace cppjit {

namespace detail {
std::string kernels_tmp_dir("./kernels_tmp/");
std::vector<void *> opened_kernel_libraries;
// std::map<std::string, void *> kernel_symbol_map;

void compile_kernel(std::string kernel_source_dir, std::string kernel_name) {
  std::string source_file(kernel_source_dir + kernel_name + ".cpp");
  std::string object_file(kernels_tmp_dir + kernel_name + ".o");
  std::string library_file(kernels_tmp_dir + "lib" + kernel_name + ".so");
  std::string compile_cmd("g++ -o " + object_file + " -c -std=c++14 -fPIC " +
                          source_file + " > " + kernel_name +
                          "_compile.log 2>&1");
  std::cout << "compile_cmd: " << compile_cmd << std::endl;
  int return_value = std::system(compile_cmd.c_str());
  if (return_value != 0) {
    std::cerr << "error: compile of kernel failed" << std::endl;
  }
  std::string link_cmd("g++ -shared -o " + library_file + " " + object_file +
                       " > " + kernel_name + "_link.log 2>&1");
  std::cout << "link_cmd: " << link_cmd << std::endl;
  return_value = std::system(link_cmd.c_str());
  if (return_value != 0) {
    std::cerr << "error: compile of kernel failed" << std::endl;
  }
}

void compile_inline_kernel(std::string kernel_inline_source,
                           std::string kernel_name) {
  std::ofstream kernel_file(kernels_tmp_dir + "src/" + kernel_name + ".cpp");
  kernel_file << kernel_inline_source;
  kernel_file.close();
  compile_kernel(kernels_tmp_dir + "/src/", kernel_name);
}
}

void *load_kernel(std::string kernel_name) {
  std::string library_file(detail::kernels_tmp_dir + "lib" + kernel_name + ".so");
  void *kernel_library = dlopen(library_file.c_str(), RTLD_LAZY);

  if (!kernel_library) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  dlerror(); /* Clear any existing error */

  // *(void **)(&kernel_impl) = dlsym(kernel_library, "kernel_impl");
  void *uncasted_function = dlsym(kernel_library, "kernel_impl");
  // detail::kernel_symbol_map[kernel_name] = uncasted_function;

  char *error = dlerror();
  if (error != nullptr) {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }

  cppjit::detail::opened_kernel_libraries.push_back(kernel_library);
  return uncasted_function;
}

void init() {}

void finalize() {
  for (void *kernel_library : detail::opened_kernel_libraries) {
    dlclose(kernel_library);
  }
  detail::opened_kernel_libraries.clear();
}
}
