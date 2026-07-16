#pragma once

#include <concepts>
#include <utility>

#include "gwen/alge/monoid.hpp"

namespace gwen {

/**
 * @brief アーベル群（逆元を持つ可換モノイド）の要件を定義するコンセプト
 * @details `monoid<T>` の要件に加えて、逆元を計算する `inv(S) -> S` 静的メンバの存在を要求する。
 * @tparam T アーベル群の要件をチェックする型
 */
template <typename T>
concept abel = monoid<T> && requires {
    { T::inv(std::declval<typename T::S>()) } -> std::convertible_to<typename T::S>;
};

/**
 * @brief 加法アーベル群。`inv(x)` は `-x` を返す。
 * @tparam T 要素型
 */
template <typename T> struct sum_abel : sum_monoid<T> {
    using S = typename sum_monoid<T>::S;
    /**
     * @brief 加法の逆元（単項マイナス）を返す
     * @param x 逆元を求める値
     * @return `-x`
     */
    static constexpr S inv(S x) { return -x; }
};

/**
 * @brief XOR アーベル群。`inv(x)` は `x` を返す（XORは自己逆元）。
 * @tparam T 要素型
 */
template <typename T> struct xor_abel : xor_monoid<T> {
    using S = typename xor_monoid<T>::S;
    /**
     * @brief XORの逆元（自己自身）を返す
     * @param x 逆元を求める値
     * @return `x`
     */
    static constexpr S inv(S x) { return x; }
};

}  // namespace gwen
