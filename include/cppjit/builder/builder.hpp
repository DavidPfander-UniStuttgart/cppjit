#pragma once

#include <dlfcn.h>
#include <fstream>
#include <sstream>

#include "../cppjit_exception.hpp"
#include "../helper.hpp"

#define DEFAULT_KERNEL_COMPILE_DIR "./cppjit_tmp/"

namespace cppjit {

// constexpr std::string DEFAULT_KERNEL_COMPILE_DIR = "./cppjit_tmp/";

namespace builder {

class builder {
protected:
  std::string kernel_name;
  bool verbose;

  bool has_source_;
  bool has_inline_source_;
  std::string source;
  std::string source_dir;
  std::string compile_dir;

  void *kernel_library;

public:
  builder(const std::string &kernel_name, bool verbose = false)
      : kernel_name(kernel_name), verbose(verbose), has_source_(false),
        has_inline_source_(false), compile_dir(DEFAULT_KERNEL_COMPILE_DIR),
        // source_dir(DEFAULT_KERNEL_COMPILE_DIR),
        kernel_library(nullptr) {}

  ~builder() {
    if (kernel_library) {
      dlclose(kernel_library);
      kernel_library = nullptr;
    }
  }

  virtual void *compile_impl() = 0;

  void *compile(const std::string &source_dir) {
    set_source_dir(source_dir);

    return compile();
  }

  void *compile() {
    if (!has_source_) {
      throw cppjit_exception("source not set");
    }
    // make sure it actually exists
    make_compile_dir();

    return compile_impl();
  };

  void *compile_inline(const std::string &kernel_inline_source) {
    set_source_inline(kernel_inline_source);
    std::ofstream kernel_file(compile_dir + kernel_name + ".cpp");
    kernel_file << kernel_inline_source;
    kernel_file.close();
    source_dir = compile_dir;

    return compile();
  }

  void make_compile_dir() {
    // create the compile dir if it doesn't exist
    std::string create_dir_cmd("mkdir -p " + compile_dir);
    int return_value = std::system(create_dir_cmd.c_str());
    if (return_value != 0) {
      throw cppjit_exception("could not create path: " + compile_dir);
    }
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

    if (kernel_library) {
      dlclose(kernel_library);
      kernel_library = nullptr;
    }

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

  void set_source_inline(const std::string &source_) {
    has_source_ = true;
    has_inline_source_ = true;
    source = source_;
  }

  void set_source_dir(const std::string &source_dir_) {
    has_source_ = true;
    has_inline_source_ = false;
    source_dir = detail::add_slash_if_missing(source_dir_);
    std::cout << "builder source_dir: " << source_dir << std::endl;
  }

  const std::string &get_source_dir() {
    if (has_inline_source_) {
      throw cppjit_exception("inline kernels do not have source directories");
    }
    return source_dir;
  }

  bool has_source() { return has_source_; }

  bool has_inline_source() { return has_inline_source_; }
};

} // namespace builder
} // namespace cppjit
