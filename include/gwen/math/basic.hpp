#pragma once
#include <concepts>
#include <bit>

namespace gwen {

/**
 * @brief 整数平方根 (floor(sqrt(n))) を計算する
 * @tparam T 符号なし整数型
 * @param n 平方根を求める対象の値
 * @return T n の平方根の切り捨て値
 */
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3605r0.pdf
template <std::unsigned_integral T>
constexpr T isqrt(const T n) noexcept {
    if (n <= T{1}) return n;
    T i_current{0}, i_next{T(T{1} << ((std::bit_width(T(n - 1)) + 1) >> 1))};
    do {
        i_current = i_next;
        i_next = T((i_current + n / i_current) >> 1);
    } while (i_next < i_current);
    return i_current;
}

/**
 * @brief x の 2 乗 (x * x) を計算する
 * @tparam T 数値型
 * @param x 対象の値
 * @return T 2 乗された値
 */
template <typename T>
constexpr T sq(T x) {
    return x * x;
}

/**
 * @brief x 個から 2 つ選ぶ組み合わせの数 (x C 2) を計算する
 * @tparam T 整数型
 * @param x 全体の個数
 * @return T 組み合わせの数
 */
template <std::integral T>
constexpr T choose2(T x) {
    return x * (x - 1) / 2;
}

} // namespace gwen
