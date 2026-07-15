#pragma once
#include <concepts>

namespace gwen {

/**
 * @brief bit の i 番目のビットを取得する
 * @tparam T1 対象となる整数の型
 * @tparam T2 ビット位置を示す整数の型
 * @param bit 対象の整数
 * @param i 取得するビットの位置 (0-indexed)
 * @return T1 i番目のビットのみを残した値（0 または 1<<i）
 */
template <std::integral T1, std::integral T2>
constexpr T1 getBit(T1 bit, T2 i) {
    return bit & (static_cast<T1>(1) << i);
}

/**
 * @brief bit の i 番目のビットを 1 に設定する
 * @tparam T1 対象となる整数の型
 * @tparam T2 ビット位置を示す整数の型
 * @param bit 対象の整数
 * @param i 設定するビットの位置 (0-indexed)
 * @return T1 設定後の整数
 */
template <std::integral T1, std::integral T2>
constexpr T1 setBit(T1 bit, T2 i) {
    return bit | (static_cast<T1>(1) << i);
}

/**
 * @brief bit の i 番目のビットを 0 にクリアする
 * @tparam T1 対象となる整数の型
 * @tparam T2 ビット位置を示す整数の型
 * @param bit 対象の整数
 * @param i クリアするビットの位置 (0-indexed)
 * @return T1 クリア後の整数
 */
template <std::integral T1, std::integral T2>
constexpr T1 clearBit(T1 bit, T2 i) {
    return bit & ~(static_cast<T1>(1) << i);
}

/**
 * @brief bit の i 番目のビットを反転させる
 * @tparam T1 対象となる整数の型
 * @tparam T2 ビット位置を示す整数の型
 * @param bit 対象の整数
 * @param i 反転するビットの位置 (0-indexed)
 * @return T1 反転後の整数
 */
template <std::integral T1, std::integral T2>
constexpr T1 toggleBit(T1 bit, T2 i) {
    return bit ^ (static_cast<T1>(1) << i);
}

} // namespace gwen
