#pragma once

#include <concepts>

namespace gwen {

/**
 * @brief 半環（Semiring）の要件を定義するコンセプト
 * @details 加法、乗法、加法単位元 `T(0)`、乗法単位元 `T(1)` の構文要件を定義する。
 * 加法の可換律、結合律、単位律、分配律、0の吸収律は利用者が保証する意味的要件である。
 * @tparam T 半環の要件をチェックする型
 */
template <typename T>
concept semiring = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { T(0) } -> std::same_as<T>;
    { T(1) } -> std::same_as<T>;
};

}  // namespace gwen
