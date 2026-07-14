#pragma once

#include <concepts>
#include <utility>

#include "gwen/alge/monoid.hpp"

namespace gwen {

template <typename T>
concept abel = monoid<T> && requires {
    { T::inv(std::declval<typename T::S>()) } -> std::convertible_to<typename T::S>;
};

template <typename T> struct sum_abel : sum_monoid<T> {
    using S = typename sum_monoid<T>::S;
    static constexpr S inv(S x) { return -x; }
};

template <typename T> struct xor_abel : xor_monoid<T> {
    using S = typename xor_monoid<T>::S;
    static constexpr S inv(S x) { return x; }
};

}  // namespace gwen
