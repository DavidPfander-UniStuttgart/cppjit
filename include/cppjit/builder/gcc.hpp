#pragma once

#include "../cppjit_exception.hpp"
#include "builder.hpp"

namespace cppjit {
namespace builder {

class gcc : public builder {
public:
  gcc(const std::string &kernel_name) : builder(kernel_name) {}

  void *compile_kernel(const std::string &source_dir) override {
    // std::string kernel_source_dir = kernel_compile_dir;
    std::string source_file(source_dir + kernel_name + ".cpp");
    std::string object_file(compile_dir + kernel_name + ".o");
    std::string library_file(compile_dir + "lib" + kernel_name + ".so");
    std::string compile_cmd(
        compiler + " -o " + object_file + " -c " + cpp_flags + " " +
        include_paths + " " + source_file + " " + library_paths + " " +
        libraries + " > " + compile_dir + kernel_name + "_compile.log 2>&1");

    if (verbose) {
      std::cout << "compile_cmd: " << compile_cmd << std::endl;
    }
    int return_value = std::system(compile_cmd.c_str());
    if (return_value != 0) {
      if (verbose) {
        std::cerr << "compile log:" << std::endl;
        read_and_print_log(compile_dir + kernel_name + "_compile.log");
      }
      throw cppjit_exception("error: compilation of kernel failed");
    }
    std::string link_cmd(linker + " " + link_flags + " -o " + library_file +
                         " " + object_file + " > " + compile_dir + kernel_name +
                         "_link.log 2>&1");
    if (verbose) {
      std::cout << "link_cmd: " << link_cmd << std::endl;
    }
    return_value = std::system(link_cmd.c_str());
    if (return_value != 0) {
      if (verbose) {
        std::cerr << "link log:" << std::endl;
        read_and_print_log(compile_dir + kernel_name + "_link.log");
      }
      throw cppjit_exception("error: compile of kernel failed");
    }
    return this->load_kernel();
  }

  const std::string &get_compiler() { return compiler; }

  void set_compiler(const std::string &compiler) { this->compiler = compiler; }

  const std::string &get_linker() { return linker; }

  void set_linker(const std::string &linker) { this->linker = linker; }

  const std::string &get_cpp_flags() { return cpp_flags; }

  void set_cpp_flags(const std::string &cpp_flags) { this->cpp_flags = cpp_flags; }

  const std::string &get_link_flags() { return link_flags; }

  void set_link_flags(const std::string &link_flags) {
    this->link_flags = link_flags;
  }

  const std::string &get_include_paths() { return include_paths; }

  void set_include_paths(const std::string &include_paths) {
    this->include_paths = include_paths;
  }

  const std::string &get_library_paths() { return library_paths; }

  void set_library_paths(const std::string &library_paths) {
    this->library_paths = library_paths;
  }

  const std::string &get_libraries() { return libraries; }

  void set_libraries(const std::string &libraries) { this->libraries = libraries; }

private:
  std::string compiler = "g++";
  std::string linker = "g++";
  std::string cpp_flags = "-std=c++14 -fPIC";
  std::string link_flags = "-shared";
  std::string include_paths = "";
  std::string library_paths = "";
  std::string libraries = "";
};

} // namespace builder
} // namespace cppjit
