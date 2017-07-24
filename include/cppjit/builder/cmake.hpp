#pragma once

#include "../cppjit_exception.hpp"
#include "builder.hpp"

namespace cppjit {
namespace builder {

class cmake : public builder {
public:
  cmake(const std::string &kernel_name) : builder(kernel_name) {}

  void *compile_impl() override {

    std::string cmake_call("cd " + compile_dir + " && " + cmake_binary + " " +
                           cmake_options + " " + source_dir + " > " +
                           kernel_name + "_cmake.log 2>&1");
    if (verbose) {
      std::cout << "cmake call: " << cmake_call << std::endl;
    }
    int return_value = std::system(cmake_call.c_str());
    if (return_value != 0) {
      if (verbose) {
        std::cerr << "cmake call log:" << std::endl;
        read_and_print_log(compile_dir + kernel_name + "_cmake.log");
      }
      throw cppjit_exception("error: cmake invocation lead to errors");
    }

    std::string make_call("cd " + compile_dir + " && " + make_binary + " " +
                          make_options + " > " + kernel_name +
                          "_make.log 2>&1");
    if (verbose) {
      std::cout << "make call: " << make_call << std::endl;
    }
    return_value = std::system(make_call.c_str());
    if (return_value != 0) {
      if (verbose) {
        std::cerr << "make call log:" << std::endl;
        read_and_print_log(compile_dir + kernel_name + "_make.log");
      }
      throw cppjit_exception("error: make invocation lead to errors");
    }

    return this->load_kernel();
  }

private:
  std::string cmake_binary = "cmake";
  std::string cmake_options;
  std::string make_binary = "make";
  std::string make_options = "";
};

} // namespace builder
} // namespace cppjit
