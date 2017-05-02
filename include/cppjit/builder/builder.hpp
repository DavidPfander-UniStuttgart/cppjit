#pragma once

#include "../cppjit_exception.hpp"

#define DEFAULT_KERNEL_COMPILE_DIR "./cppjit_tmp/"

namespace cppjit {
namespace builder {

class builder {
public:
  builder(const std::string &kernel_name, bool verbose = false)
      : kernel_name(kernel_name), verbose(verbose),
        compile_dir(DEFAULT_KERNEL_COMPILE_DIR),
        // source_dir(DEFAULT_KERNEL_COMPILE_DIR),
        kernel_library(nullptr) {}

  ~builder() {
    if (kernel_library) {
      dlclose(kernel_library);
    }
  }

  virtual void *compile_kernel(const std::string &source_dir) = 0;

  void *compile_kernel() { return compile_kernel(compile_dir); };

  void *compile_inline_kernel(std::string kernel_inline_source) {
    // source_dir = compile_dir;
    std::ofstream kernel_file(compile_dir + kernel_name + ".cpp");
    kernel_file << kernel_inline_source;
    kernel_file.close();
    return compile_kernel(compile_dir);
  }

  void read_and_print_log(std::string path) {
    std::ifstream f;
    f.open(path);
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

  void *load_kernel() {
    std::string library_file(compile_dir + "lib" + kernel_name + ".so");

    dlerror(); // clear any prior error
    kernel_library = dlopen(library_file.c_str(), RTLD_LAZY);
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
    return uncasted_function;
  }

  void set_verbose(bool verbose) { this->verbose = verbose; }

  bool is_verbose() { return verbose; }

  void set_compile_dir(std::string path, bool create_directory = false) {
    if (path.size() == 0) {
      throw cppjit_exception("specified temporary dir cannot be empty");
    }
    // create directory if set
    if (create_directory) {
      std::string create_dir_cmd("mkdir -p " + path);
      int return_value = std::system(create_dir_cmd.c_str());
      if (return_value != 0) {
        throw cppjit_exception("could not create path: " + path);
      }
    }
    // check whether path exists and is a directory
    std::string dir_check_cmd("test -d " + path);
    int return_value = std::system(dir_check_cmd.c_str());
    if (return_value != 0) {
      throw cppjit_exception("specified temporary path does not exist: " +
                             path);
    }

    // normalize path
    if (path[path.size() - 1] != '/') {
      path = path + "/";
    }
    compile_dir = path;
  }

  const std::string &get_compile_dir() { return compile_dir; }

public:
  std::string kernel_name;
  bool verbose;

  std::string compile_dir;
  // std::string source_dir;

  void *kernel_library;
};

} // namespace builder
} // namespace cppjit
