#pragma once

#include <algorithm>
#include <concepts>
#include <limits>
#include <numeric>
#include <utility>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief モノイドの要件を定義するコンセプト
 * @details 型 T が型エイリアス S、二項演算 op(S,S)->S、単位元 e()->S を持つことを要求する。
 * @tparam T モノイドの要件をチェックする型
 */
template <typename T>
concept monoid = requires {
    typename T::S;
    { T::op(std::declval<typename T::S>(), std::declval<typename T::S>()) } -> std::convertible_to<typename T::S>;
    { T::e() } -> std::convertible_to<typename T::S>;
};

/**
 * @brief 何も操作を行わないダミーモノイド
 * @tparam T 要素型
 */
template <typename T> struct no_op_monoid {
    using S = T;
    static constexpr S op(S, S) { return e(); }
    static constexpr S e() { return S(); }
    using F = T;
    static constexpr S mapping(F, S) { return e(); }
    static constexpr F composition(F, F) { return id(); }
    static constexpr F id() { return F(); }
};

/**
 * @brief 加法モノイド。op(a, b) = a + b、e() = 0。
 * @tparam T 要素型
 */
template <typename T> struct sum_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return a + b; }
    static constexpr S e() { return 0; }
};

/**
 * @brief XORモノイド。op(a, b) = a ^ b、e() = 0。
 * @tparam T 要素型
 */
template <typename T> struct xor_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return a ^ b; }
    static constexpr S e() { return 0; }
};

/**
 * @brief 最小値モノイド。op(a, b) = min(a, b)、e() = 型の最大値。
 * @tparam T 要素型
 */
template <typename T> struct min_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::min(a, b); }
    static constexpr S e() { return std::numeric_limits<S>::max(); }
};

/**
 * @brief 最大値モノイド。op(a, b) = max(a, b)、e() = 型の最小値。
 * @tparam T 要素型
 */
template <typename T> struct max_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::max(a, b); }
    static constexpr S e() { return std::numeric_limits<S>::min(); }
};

/**
 * @brief 最小値と最大値のペアを管理するモノイド。
 * @tparam T 要素型
 */
template <typename T> struct minmax_monoid {
    struct S {
        i32 min, max;
    };
    static constexpr S op(S a, S b) { return {std::min(a.min, b.min), std::max(a.max, b.max)}; }
    static constexpr S e() { return {std::numeric_limits<T>::max(), std::numeric_limits<T>::min()}; }
};

/**
 * @brief 最大公約数モノイド。op(a, b) = gcd(a, b)、e() = 0。
 * @tparam T 要素型
 */
template <typename T> struct gcd_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::gcd(a, b); }
    static constexpr S e() { return 0; }
};

/**
 * @brief 一次関数 f(x) = ax + b の合成を管理するモノイド。
 * @details op(f, g)(x) = f(g(x)) = f.a * g.a * x + f.a * g.b + f.b
 * @tparam T 係数型
 */
template <typename T> struct affine_monoid {
    struct S {
        T a, b;
    };
    static constexpr S op(S f, S g) { return {f.a * g.a, f.a * g.b + f.b}; }
    static constexpr S e() { return {1, 0}; }
};

}  // namespace gwen
