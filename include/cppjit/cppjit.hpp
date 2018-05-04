#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include "builder/builder.hpp"
#include "builder/gcc.hpp"
#include "cppjit_exception.hpp"
#include "function_traits.hpp"

namespace cppjit {

// base template for the following specialization
// required to do the pack-matching in the specialization
template <typename... Args> struct kernel;

template <typename R, typename... Args>
class kernel<R, cppjit::detail::pack<Args...>> {
private:
  std::string kernel_name;
  std::shared_ptr<builder::builder> builder;
  std::function<R(Args...)> kernel_implementation;
  bool verbose;

public:
  kernel(const std::string &kernel_name)
      : kernel_name(kernel_name),
        builder(std::make_shared<cppjit::builder::gcc>(kernel_name)),
        verbose(false) {}

  kernel(const std::string &kernel_name, const std::string &kernel_src_dir)
      : kernel_name(kernel_name),
        builder(std::make_shared<cppjit::builder::gcc>(kernel_name)),
        verbose(false) {
    set_source_dir(kernel_src_dir);
  }

  R operator()(Args... args) {
    if (!kernel_implementation) {
      this->compile();
      // throw cppjit::cppjit_exception("kernel not compiled");
    }
    return kernel_implementation(std::forward<Args>(args)...);
  }

  void compile() {
    void *uncasted_function = builder->compile();
    R (*fp)(Args...) = reinterpret_cast<decltype(fp)>(uncasted_function);
    kernel_implementation = fp;
  }

  void compile(const std::string &source_dir) {
    void *uncasted_function = builder->compile(source_dir);
    R (*fp)(Args...) = reinterpret_cast<decltype(fp)>(uncasted_function);
    kernel_implementation = fp;
  }

  void compile_inline(const std::string &source) {
    void *uncasted_function = builder->compile_inline(source);
    R (*fp)(Args...) = reinterpret_cast<decltype(fp)>(uncasted_function);
    kernel_implementation = fp;
  }

  bool has_builder() { return this->builder; }

  template <class builder_class> builder_class &get_builder() {
    return *std::dynamic_pointer_cast<builder_class>(builder);
  }

  bool is_compiled() {
    if (kernel_implementation) {
      return true;
    }
    return false;
  }

  void set_verbose(bool verbose) {
    this->verbose = verbose;
    if (builder) {
      builder->set_builder_verbose(verbose);
    }
  }

  bool is_verbose() { return verbose; }

  void set_builder(std::shared_ptr<cppjit::builder::builder> builder_) {
    if (!builder) {
      throw cppjit::cppjit_exception("builder is invalid");
    }
    // TODO: improve
    builder->invalidate();
    builder = builder_;
  }
  std::shared_ptr<cppjit::builder::builder> get_builder() { return builder; }

  // have to specify new builder manually after clear
  void clear() {
    kernel_implementation = nullptr;
    if (builder) {
      std::string old_source_dir;
      if (!this->has_inline_source()) {
        old_source_dir = builder->get_source_dir();
      }
      builder->invalidate();
      builder = std::make_shared<cppjit::builder::gcc>(kernel_name);
      if (!this->has_inline_source()) {
        builder->set_source_dir(old_source_dir);
      }
    }
  }
  void set_source_inline(const std::string &source_) {
    builder->set_source_inline(source_);
  }

  void set_source_dir(const std::string &source_dir_) {
    builder->set_source_dir(source_dir_);
  }
  bool has_source() {
    if (!builder) {
      throw cppjit::cppjit_exception("builder is invalid");
    }
    return builder->has_source();
  }

  bool has_inline_source() {
    if (!builder) {
      throw cppjit::cppjit_exception("builder is invalid");
    }
    return builder->has_inline_source();
  }
};
}

#define CPPJIT_DECLARE_KERNEL(kernel_signature, kernel_name)                   \
  namespace cppjit {                                                           \
  extern kernel<                                                               \
      cppjit::detail::function_traits<kernel_signature>::return_type,          \
      cppjit::detail::function_traits<kernel_signature>::args_type>            \
      kernel_name;                                                             \
  }

#define CPPJIT_DEFINE_KERNEL_NO_SRC(kernel_signature, kernel_name)             \
  namespace cppjit {                                                           \
  kernel<cppjit::detail::function_traits<kernel_signature>::return_type,       \
         cppjit::detail::function_traits<kernel_signature>::args_type>         \
      kernel_name(#kernel_name);                                               \
  }

#define CPPJIT_KERNEL_NO_SRC(kernel_signature, kernel_name)                    \
  CPPJIT_DECLARE_KERNEL(kernel_signature, kernel_name)                         \
  CPPJIT_DEFINE_KERNEL_NO_SRC(kernel_signature, kernel_name)

#define CPPJIT_DEFINE_KERNEL(kernel_signature, kernel_name, kernel_src_dir)    \
  namespace cppjit {                                                           \
  kernel<cppjit::detail::function_traits<kernel_signature>::return_type,       \
         cppjit::detail::function_traits<kernel_signature>::args_type>         \
      kernel_name(#kernel_name, kernel_src_dir);                               \
  }

#define CPPJIT_KERNEL(kernel_signature, kernel_name, kernel_src_dir)           \
  CPPJIT_DECLARE_KERNEL(kernel_signature, kernel_name)                         \
  CPPJIT_DEFINE_KERNEL(kernel_signature, kernel_name, kernel_src_dir)
