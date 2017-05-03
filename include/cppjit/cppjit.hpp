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

#define CPPJIT_DECLARE_KERNEL(kernel_signature, kernel_name)                   \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  extern std::function<kernel_signature> kernel_name;                          \
  }                                                                            \
  namespace builder {                                                          \
  extern std::shared_ptr<cppjit::builder::builder> kernel_name;                \
  }                                                                            \
  namespace compiler_inline {                                                  \
  template <typename... Args> struct kernel_name;                              \
  template <typename R, typename... Args>                                      \
  struct kernel_name<R, cppjit::detail::pack<Args...>> {                       \
    void operator()(const std::string &source) {                               \
      if (!cppjit::builder::kernel_name) {                                     \
        cppjit::builder::kernel_name =                                         \
            std::make_shared<cppjit::builder::gcc>(#kernel_name);              \
      }                                                                        \
      if (cppjit::builder::kernel_name->is_verbose()) {                        \
        std::cout << "kernel \"" << #kernel_name                               \
                  << "\" not initialized: initializing..." << std::endl;       \
      }                                                                        \
      void *uncasted_function =                                                \
          cppjit::builder::kernel_name->compile_inline_kernel(source);         \
      R (*fp)(Args...) = reinterpret_cast<decltype(fp)>(uncasted_function);    \
      cppjit::kernels::kernel_name = fp;                                       \
    }                                                                          \
  };                                                                           \
  }                                                                            \
  }                                                                            \
  cppjit::compiler_inline::kernel_name<                                        \
      cppjit::detail::function_traits<kernel_signature>::return_type,          \
      cppjit::detail::function_traits<kernel_signature>::args_type>            \
      compile_inline_##kernel_name;                                            \
  namespace cppjit {                                                           \
  namespace compiler {                                                         \
  template <typename... Args> struct kernel_name;                              \
  template <typename R, typename... Args>                                      \
  struct kernel_name<R, cppjit::detail::pack<Args...>> {                       \
    void operator()(const std::string &source_dir = "") {                      \
      if (!cppjit::builder::kernel_name) {                                     \
        cppjit::builder::kernel_name =                                         \
            std::make_shared<cppjit::builder::gcc>(#kernel_name);              \
      }                                                                        \
      if (cppjit::builder::kernel_name->is_verbose()) {                        \
        std::cout << "kernel \"" << #kernel_name                               \
                  << "\" not initialized: initializing..." << std::endl;       \
      }                                                                        \
      void *uncasted_function =                                                \
          cppjit::builder::kernel_name->compile_kernel(source_dir);            \
      R (*fp)(Args...) = reinterpret_cast<decltype(fp)>(uncasted_function);    \
      cppjit::kernels::kernel_name = fp;                                       \
    }                                                                          \
  };                                                                           \
  }                                                                            \
  }                                                                            \
  cppjit::compiler::kernel_name<                                               \
      cppjit::detail::function_traits<kernel_signature>::return_type,          \
      cppjit::detail::function_traits<kernel_signature>::args_type>            \
      compile_##kernel_name;                                                   \
  namespace cppjit {                                                           \
  namespace wrapper {                                                          \
  template <typename... Args> struct kernel_name;                              \
  template <typename R, typename... Args>                                      \
  struct kernel_name<R, cppjit::detail::pack<Args...>> {                       \
    R operator()(Args... args) {                                               \
      if (!cppjit::kernels::kernel_name) {                                     \
        throw cppjit::cppjit_exception("kernel not compiled");                 \
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
  namespace cppjit {                                                           \
  bool is_compiled_##kernel_name() {                                           \
    if (cppjit::kernels::kernel_name) {                                        \
      return true;                                                             \
    }                                                                          \
    return false;                                                              \
  }                                                                            \
  void set_builder_##kernel_name(                                              \
      std::shared_ptr<cppjit::builder::builder> builder) {                     \
    if (!builder) {                                                            \
      throw cppjit::cppjit_exception("builder is invalid");                    \
    }                                                                          \
    cppjit::builder::kernel_name = builder;                                    \
  }                                                                            \
  std::shared_ptr<cppjit::builder::builder> get_builder_##kernel_name() {      \
    if (!cppjit::builder::kernel_name) {                                       \
      cppjit::builder::kernel_name =                                           \
          std::make_shared<cppjit::builder::gcc>(#kernel_name);                \
    }                                                                          \
    return cppjit::builder::kernel_name;                                       \
  }                                                                            \
  void clear_##kernel_name() {                                                 \
    cppjit::kernels::kernel_name = nullptr;                                    \
    cppjit::builder::kernel_name = nullptr;                                    \
  }                                                                            \
  }

#define CPPJIT_DEFINE_KERNEL(kernel_signature, kernel_name)                    \
  namespace cppjit {                                                           \
  namespace kernels {                                                          \
  std::function<kernel_signature> kernel_name;                                 \
  }                                                                            \
  namespace builder {                                                          \
  std::shared_ptr<cppjit::builder::builder> kernel_name;                       \
  }                                                                            \
  }
