#pragma once

#include <concepts>

#include "gwen/alge/semiring.hpp"

namespace gwen {

/**
 * @brief 環(Ring)の要件を定義するコンセプト
 * @details `semiring<T>` の要件に加え、減算と加算・減算・乗算の複合代入演算を要求する。
 * @tparam T 環の要件をチェックする型
 */
template <typename T>
concept ring = semiring<T> && requires(T a, T b) {
    { a - b } -> std::same_as<T>;
    { a += b } -> std::same_as<T&>;
    { a -= b } -> std::same_as<T&>;
    { a *= b } -> std::same_as<T&>;
};

}  // namespace gwen
