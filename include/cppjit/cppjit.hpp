#pragma once

#include <fstream>
#include <functional>
#include <map>
#include <vector>

#include <dlfcn.h>

namespace cppjit {

namespace detail {
std::string kernels_tmp_dir("./kernels_tmp/");
std::vector<void *> opened_kernel_libraries;
// std::map<std::string, void *> kernel_symbol_map;
std::map<std::string, std::string> kernel_source_map;

void register_kernel(std::string kernel_name, std::string kernel_source) {
  kernel_source_map[kernel_name] = kernel_source;
}

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
  compile_kernel(kernels_tmp_dir + "src/", kernel_name);
}
}

void *load_kernel(std::string kernel_name) {
  std::string library_file(detail::kernels_tmp_dir + "lib" + kernel_name +
                           ".so");
  void *kernel_library = dlopen(library_file.c_str(), RTLD_LAZY);

  if (!kernel_library) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  dlerror(); /* Clear any existing error */

  // *(void **)(&kernel_impl) = dlsym(kernel_library, "kernel_impl");
  void *uncasted_function = dlsym(kernel_library, kernel_name.c_str());
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

//

// semantics: function declaration, put into header
#define CPPJIT_DECLARE_KERNEL_NEW(kernel_signature, kernel_name)               \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  extern std::function<kernel_signature> kernel_name;                          \
  }                                                                            \
  }                                                                            \
  template <typename ret, typename... Ts> ret kernel_name(Ts... args) {        \
    if (!cppjit::kernels::kernel_name) {                                       \
      std::cout << "help! not initialized" << std::endl;                       \
      cppjit::detail::compile_inline_kernel(                                   \
          cppjit::detail::kernel_source_map[#kernel_name], #kernel_name);      \
      void *uncasted_function = cppjit::load_kernel(#kernel_name);             \
      ret (*fp)(Ts...) = reinterpret_cast<decltype(fp)>(uncasted_function);    \
      cppjit::kernels::kernel_name = fp;                                       \
    }                                                                          \
    return cppjit::kernels::kernel_name(std::forward<Ts>(args)...);            \
  }

// *(void **)(&cppjit::kernels::kernel_name) = uncasted_function;
// cppjit::kernels::kernel_name =                                           \
      //     static_cast<decltype(cppjit::kernels::kernel_name)>(                 \
      //         uncasted_function);                                              \

#define CPPJIT_DECLARE_KERNEL(kernel_return_type, kernel_name,                 \
                              kernel_braced_parameter_list)                    \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  extern kernel_return_type(*kernel_name) kernel_braced_parameter_list;        \
  }                                                                            \
  }                                                                            \
  kernel_return_type kernel_name kernel_braced_parameter_list;

// semantics: like a function call, has to be called before kernel is called
#define CPPJIT_REGISTER_KERNEL_SOURCE(kernel_name, kernel_source)              \
  {                                                                            \
    std::string kernel_src_string(kernel_source);                              \
    cppjit::detail::register_kernel(kernel_name, kernel_src_string);           \
    std::cout << kernel_src_string << std::endl;                               \
  }

#define CPPJIT_DEFINE_KERNEL(kernel_return_type, kernel_name,                  \
                             kernel_braced_parameter_list, argument_list)      \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  kernel_return_type(*kernel_name) kernel_braced_parameter_list = nullptr;     \
  }                                                                            \
  }                                                                            \
  kernel_return_type kernel_name kernel_braced_parameter_list {                \
    if (cppjit::kernels::kernel_name == nullptr) {                             \
      cppjit::detail::compile_inline_kernel(                                   \
          cppjit::detail::kernel_source_map[#kernel_name], #kernel_name);      \
      void *uncasted_function = cppjit::load_kernel(#kernel_name);             \
      *(void **)(&cppjit::kernels::kernel_name) = uncasted_function;           \
    }                                                                          \
    cppjit::kernels::kernel_name argument_list;                                \
  }

#define CPPJIT_DEFINE_KERNEL_NEW(kernel_signature, kernel_name)                \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  std::function<kernel_signature> kernel_name;                                 \
  }                                                                            \
  }
