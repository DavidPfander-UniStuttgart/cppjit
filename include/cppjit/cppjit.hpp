#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <dlfcn.h>

#include "cppjit_exception.hpp"
#include "function_traits.hpp"

namespace cppjit {

namespace detail {
std::string kernels_tmp_dir("./cppjit_tmp/");
std::vector<void *> opened_kernel_libraries;
bool verbose = false;

void read_and_print_log(std::string file_name) {
  std::ifstream f;
  f.open(file_name);
  if (!f.is_open()) {
    std::cerr << "could not read log file";
    return;
  }
  std::string line;
  while (std::getline(f, line)) {
    std::cerr << line << std::endl;
  }
  f.close();
}

void compile_kernel(std::string kernel_source_dir, std::string kernel_name) {
  std::string source_file(kernel_source_dir + kernel_name + ".cpp");
  std::string object_file(kernels_tmp_dir + kernel_name + ".o");
  std::string library_file(kernels_tmp_dir + "lib" + kernel_name + ".so");
  std::string compile_cmd("g++ -o " + object_file + " -c -std=c++14 -fPIC " +
                          source_file + " > " + kernel_name +
                          "_compile.log 2>&1");
  if (verbose) {
    std::cout << "compile_cmd: " << compile_cmd << std::endl;
  }
  int return_value = std::system(compile_cmd.c_str());
  if (return_value != 0) {
    if (verbose) {
      std::cerr << "compile log:" << std::endl;
      read_and_print_log(kernel_name + "_compile.log");
    }
    throw cppjit_exception("error: compilation of kernel failed");
  }
  std::string link_cmd("g++ -shared -o " + library_file + " " + object_file +
                       " > " + kernel_name + "_link.log 2>&1");
  if (verbose) {
    std::cout << "link_cmd: " << link_cmd << std::endl;
  }
  return_value = std::system(link_cmd.c_str());
  if (return_value != 0) {
    if (verbose) {
      std::cerr << "link log:" << std::endl;
      read_and_print_log(kernel_name + "_link.log");
    }
    throw cppjit_exception("error: compile of kernel failed");
  }
}

void compile_inline_kernel(std::string kernel_inline_source,
                           std::string kernel_name) {
  std::ofstream kernel_file(kernels_tmp_dir + kernel_name + ".cpp");
  kernel_file << kernel_inline_source;
  kernel_file.close();
  compile_kernel(kernels_tmp_dir, kernel_name);
}

void *load_kernel(std::string kernel_name) {
  std::string library_file(detail::kernels_tmp_dir + "lib" + kernel_name +
                           ".so");

  dlerror(); // clear any prior error
  void *kernel_library = dlopen(library_file.c_str(), RTLD_LAZY);
  char *error = dlerror();

  if (error != nullptr) {
    std::stringstream s;
    s << "error: could not open kernel shared library file, reason: ";
    s << dlerror();
    throw cppjit_exception(s.str());
  }

  dlerror(); // clear any prior error
  void *uncasted_function = dlsym(kernel_library, kernel_name.c_str());
  error = dlerror();

  if (error != nullptr) {
    std::stringstream s;
    s << "error: could not find kernel symbol, reason: ";
    s << error;
    throw cppjit_exception(s.str());
  }

  cppjit::detail::opened_kernel_libraries.push_back(kernel_library);
  return uncasted_function;
}
}

void set_verbose(bool verbose) { cppjit::detail::verbose = verbose; }

void finalize() {
  for (void *kernel_library : detail::opened_kernel_libraries) {
    dlclose(kernel_library);
  }
  detail::opened_kernel_libraries.clear();
}
}

#define CPPJIT_DECLARE_KERNEL(kernel_signature, kernel_name)                   \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  extern std::function<kernel_signature> kernel_name;                          \
  }                                                                            \
  namespace source {                                                           \
  extern std::string kernel_name;                                              \
  }                                                                            \
  namespace loader {                                                           \
  template <typename... Args> struct kernel_name;                              \
  template <typename R, typename... Args>                                      \
  struct kernel_name<R, cppjit::detail::pack<Args...>> {                       \
    void operator()() {                                                        \
      if (cppjit::detail::verbose) {                                           \
        std::cout << "kernel \"" << #kernel_name                               \
                  << "\" not initialized: initializing..." << std::endl;       \
      }                                                                        \
      cppjit::detail::compile_inline_kernel(cppjit::source::kernel_name,       \
                                            #kernel_name);                     \
      void *uncasted_function = cppjit::detail::load_kernel(#kernel_name);     \
      R (*fp)(Args...) = reinterpret_cast<decltype(fp)>(uncasted_function);    \
      cppjit::kernels::kernel_name = fp;                                       \
    }                                                                          \
  };                                                                           \
  }                                                                            \
  }                                                                            \
  cppjit::loader::kernel_name<                                                 \
      cppjit::detail::function_traits<kernel_signature>::return_type,          \
      cppjit::detail::function_traits<kernel_signature>::args_type>            \
      load_##kernel_name;                                                      \
  namespace cppjit {                                                           \
  namespace wrapper {                                                          \
  template <typename... Args> struct kernel_name;                              \
  template <typename R, typename... Args>                                      \
  struct kernel_name<R, cppjit::detail::pack<Args...>> {                       \
    R operator()(Args... args) {                                               \
      if (!cppjit::kernels::kernel_name) {                                     \
        load_##kernel_name();                                                  \
      }                                                                        \
      return cppjit::kernels::kernel_name(std::forward<Args>(args)...);        \
    }                                                                          \
  };                                                                           \
  }                                                                            \
  }                                                                            \
  cppjit::wrapper::kernel_name<                                                \
      cppjit::detail::function_traits<kernel_signature>::return_type,          \
      cppjit::detail::function_traits<kernel_signature>::args_type>            \
      kernel_name;                                                             \
  void set_source_##kernel_name(std::string source) {                          \
    cppjit::kernels::kernel_name = nullptr;                                    \
    cppjit::source::kernel_name = source;                                      \
  }                                                                            \
  bool is_loaded_##kernel_name() {                                             \
    if (cppjit::kernels::kernel_name) {                                        \
      return true;                                                             \
    }                                                                          \
    return false;                                                              \
  }

#define CPPJIT_DEFINE_KERNEL(kernel_signature, kernel_name)                    \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  std::function<kernel_signature> kernel_name;                                 \
  }                                                                            \
  namespace source {                                                           \
  std::string kernel_name;                                                     \
  }                                                                            \
  }
