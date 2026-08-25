#pragma once

#include <algorithm>
#include <concepts>
#include <limits>
#include <numeric>
#include <utility>
#include <variant>

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
 */
struct no_op_monoid {
    using S = std::monostate;
    using F = std::monostate;

    /** @brief 唯一の要素を結合する */
    static constexpr S op(S, S) { return {}; }

    /** @brief 唯一の要素を単位元として返す */
    static constexpr S e() { return {}; }

    /** @brief 恒等作用を唯一の要素へ適用する */
    static constexpr S mapping(F, S) { return {}; }

    /** @brief 恒等作用同士を合成する */
    static constexpr F composition(F, F) { return {}; }

    /** @brief 唯一の作用素を恒等作用として返す */
    static constexpr F id() { return {}; }
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
 * @brief 最小値モノイド。op(a, b) = min(a, b)、e() = 正の無限大または型の最大値。
 * @tparam T 要素型
 */
template <typename T> struct min_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::min(a, b); }

    /** @brief 浮動小数点型では正の無限大、それ以外では型の最大値を単位元として返す */
    static constexpr S e() {
        if constexpr (std::numeric_limits<S>::has_infinity) return std::numeric_limits<S>::infinity();
        return std::numeric_limits<S>::max();
    }
};

/**
 * @brief 最大値モノイド。op(a, b) = max(a, b)、e() = 負の無限大または型の最小値。
 * @tparam T 要素型
 */
template <typename T> struct max_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return std::max(a, b); }

    /** @brief 浮動小数点型では負の無限大、それ以外では型の最小値を単位元として返す */
    static constexpr S e() {
        if constexpr (std::numeric_limits<S>::has_infinity) return -std::numeric_limits<S>::infinity();
        return std::numeric_limits<S>::lowest();
    }
};

/**
 * @brief 最小値と最大値のペアを管理するモノイド。
 * @tparam T 要素型
 */
template <typename T> struct minmax_monoid {
    struct S {
        T min, max;
    };
    static constexpr S op(S a, S b) { return {std::min(a.min, b.min), std::max(a.max, b.max)}; }

    /** @brief 最小値モノイドと最大値モノイドの単位元の組を返す */
    static constexpr S e() { return {min_monoid<T>::e(), max_monoid<T>::e()}; }
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

/**
 * @brief 乗法モノイド。op(a, b) = a * b、e() = 1。
 * @tparam T 要素型
 */
template <typename T> struct mul_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return a * b; }
    static constexpr S e() { return S(1); }
};

/**
 * @brief ビットANDモノイド。op(a, b) = a & b、e() = 全ビット1。
 * @tparam T 要素型
 */
template <typename T> struct and_monoid {
    using S = T;
    static constexpr S op(S a, S b) { return a & b; }

    /** @brief 全ビット1を単位元として返す */
    static constexpr S e() { return ~S(0); }
};

}  // namespace gwen
