#pragma once

#include <algorithm>
#include <concepts>
#include <limits>
#include <numeric>
#include <utility>

#include "gwen/types.hpp"

namespace gwen {

template <typename T>
concept monoid = requires {
    typename T::S;
    { T::op(std::declval<typename T::S>(), std::declval<typename T::S>()) } -> std::convertible_to<typename T::S>;
    { T::e() } -> std::convertible_to<typename T::S>;
};

template <typename T> struct no_op_monoid {
    using S = T;
    static constexpr S op(S, S) { return e(); }
    static constexpr S e() { return S(); }
    using F = T;
    static constexpr S mapping(F, S) { return e(); }
    static constexpr F composition(F, F) { return id(); }
    static constexpr F id() { return F(); }
};

template <typename T> struct sum_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return a + b; }
    static constexpr S e() { return 0; }
};

template <typename T> struct xor_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return a ^ b; }
    static constexpr S e() { return 0; }
};

template <typename T> struct min_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::min(a, b); }
    static constexpr S e() { return std::numeric_limits<S>::max(); }
};

template <typename T> struct max_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::max(a, b); }
    static constexpr S e() { return std::numeric_limits<S>::min(); }
};

template <typename T> struct minmax_monoid {
    struct S {
        i32 min, max;
    };
    static constexpr S op(S a, S b) { return {std::min(a.min, b.min), std::max(a.max, b.max)}; }
    static constexpr S e() { return {std::numeric_limits<T>::max(), std::numeric_limits<T>::min()}; }
};

template <typename T> struct gcd_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::gcd(a, b); }
    static constexpr S e() { return 0; }
};

template <typename T> struct affine_monoid {
    struct S {
        T a, b;
    };
    static constexpr S op(S f, S g) { return {f.a * g.a, f.a * g.b + f.b}; }
    static constexpr S e() { return {1, 0}; }
};

}  // namespace gwen
