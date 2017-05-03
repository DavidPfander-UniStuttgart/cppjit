# cppjit

cppjit is a simple, easy-to-use c++ just-in-time compiler (frontend)

# Features

* transparent interface, very easy to use:
```
#include "cppjit/cppjit.hpp"

// declares kernel, provide function type and a c-identifier as name
CPPJIT_DECLARE_KERNEL(void(), kernel)

int main(void) {

  // set source, compile and load kernel
  compile_inline_kernel(
      "#include <iostream>\n extern \"C\" void "
      "kernel() { std::cout << \"hello jit world\" << std::endl;}");

  // call kernel like an other function
  kernel();
}

// defines kernel, put in single compilation unit
CPPJIT_DEFINE_KERNEL(void(), kernel)
```
* kernels can be inlined (as above) or provided as files
* supports gcc and cmake as backends to build kernels at application runtime
* header-only, no compilation required (except for examples and tests) 
* permissive license, can be copied into another application
* support for unix-like operating systems
* requires a C++14 compiler (might work with C++11)

# How does it work?
* implements as a wrapper around dlopen(), dlsym(), dlclose(), should work on platforms where those are available
* because of this, the kernel function has to have `extern "C"` linkage:
  * arguments and return value can still be arbitrary C++ types
  * classes are supported, see examples

# Bugs or features requests
* just open a ticket on GitHub

# How to use it?
* see the examples folder for some basic use cases

## Circle-Ci

The [CircleCI](https://circleci.com/gh/STEllAR-GROUP/octotiger) contiguous
integration service tracks the current build status for the master branch: [![link](https://circleci.com/gh/DavidPfander-UniStuttgart/cppjit/tree/master.svg?style=svg)](https://circleci.com/gh/DavidPfander-UniStuttgart/cppjit/tree/master)
