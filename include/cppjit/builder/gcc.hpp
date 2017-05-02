#pragma once

#include "../cppjit_exception.hpp"
#include "builder.hpp"

namespace cppjit {
namespace builder {

class GCC : public builder {
public:
  GCC(const std::string &kernel_name) : builder(kernel_name) {}

  void *compile_kernel(const std::string &source_dir) override {
    // std::string kernel_source_dir = kernel_compile_dir;
    std::string source_file(source_dir + kernel_name + ".cpp");
    std::string object_file(compile_dir + kernel_name + ".o");
    std::string library_file(compile_dir + "lib" + kernel_name + ".so");
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
    return this->load_kernel();
  };
};

} // namespace builder
} // namespace cppjit
