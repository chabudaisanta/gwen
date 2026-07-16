#pragma once
#include <cassert>
#include <concepts>

namespace gwen {

/**
 * @brief x / y の切り上げ除算を行う
 * @tparam T 整数型
 * @param x 割られる数
 * @param y 割る数 (0であってはならない)
 * @return T 切り上げ除算の結果
 */
template <std::integral T> constexpr T ceil_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d + ((x > 0) ^ (y < 0));
}

/**
 * @brief x / y の切り捨て除算を行う
 * @tparam T 整数型
 * @param x 割られる数
 * @param y 割る数 (0であってはならない)
 * @return T 切り捨て除算の結果
 */
template <std::integral T> constexpr T floor_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : d - ((x < 0) ^ (y < 0));
}

/**
 * @brief x / y の結果を 0 から遠ざかる方向に丸める
 * @tparam T 整数型
 * @param x 割られる数
 * @param y 割る数 (0であってはならない)
 * @return T 外側への丸め除算の結果
 */
template <std::integral T> constexpr T out_div(T x, T y) {
    assert(y != 0);
    T d = x / y;
    return d * y == x ? d : ((x > 0) == (y > 0)) ? d + 1 : d - 1;
}

}  // namespace gwen
