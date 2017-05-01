#pragma once

#include "my_class_interface.hpp"

class my_class : public my_class_interface {
public:
  virtual bool f1(int i) override {
    if (i == 5) {
      return true;
    }
    return false;
  }
  virtual bool f2(char c) override {
    if (c == 'b') {
      return true;
    }
    return false;
  }
};
