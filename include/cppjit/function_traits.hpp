#pragma once

#include <cstddef>
#include <functional>
#include <tuple>

namespace cppjit {
namespace detail {

// empty class for representing parameter pack's indirectly (directly is missing
// C++ feature)
template <class... Args> struct pack;

// forward declaration for following specialization
template <class F> struct function_traits;

// using specialization to match for function-type -> use this overload for
// function type
// this overload works for function pointers
template <class R, class... Args> struct function_traits<R(Args...)> {

  // expands to return type
  using return_type = R;

  // expands to number of arguemnts
  static constexpr std::size_t arity = sizeof...(Args);

  // expand to type of nth argument
  template <std::size_t N>
  using arg_type = typename std::tuple_element<N, std::tuple<Args...>>::type;

  // to be used in other templates by matching pack's paramters -> gives the
  // argument list
  using args_type = pack<Args...>;
};

// overload for std::function -> works by unpacking inner type of std::function
// can reuse above type evaluation through inheritance
template <class R, class... Args>
struct function_traits<std::function<R(Args...)>>
    : public function_traits<R(Args...)> {};

// overload for functino pointer, same idea as for std::function
template <class R, class... Args>
struct function_traits<R (*)(Args...)> : public function_traits<R(Args...)> {};

} // namespace detail
} // namespace cppjit
