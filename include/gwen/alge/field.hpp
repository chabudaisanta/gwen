#pragma once

#include <concepts>

#include "gwen/alge/ring.hpp"

namespace gwen {

/**
 * @brief 体 (Field) の要件を定義するコンセプト
 * @details `ring<T>` の要件に加え、単項マイナス、元同士の等値比較、および除算・除算代入演算を要求する。
 * 加減乗除の代数法則（0以外の元の乗法逆元の存在、可換律等）は利用者が保証する意味的要件である。
 * @tparam T 体の要件をチェックする型
 */
template <typename T>
concept field = ring<T> && std::equality_comparable<T> && requires(T a, T b) {
    { -a } -> std::same_as<T>;
    { a / b } -> std::same_as<T>;
    { a /= b } -> std::same_as<T&>;
};

}  // namespace gwen
