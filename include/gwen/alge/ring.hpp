#pragma once

#include <concepts>

namespace gwen {

/**
 * @brief 環(Ring)の要件を定義するコンセプト
 */
template <typename T>
concept ring = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { a += b } -> std::same_as<T&>;
    { a -= b } -> std::same_as<T&>;
    { a *= b } -> std::same_as<T&>;
};

}  // namespace gwen
