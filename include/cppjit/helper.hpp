#pragma once

namespace cppjit {
namespace detail {
inline std::string add_slash_if_missing(const std::string &input) {
  if (input[input.size() - 1] != '/') {
    return input + '/';
  }
  return input;
}
}
}
