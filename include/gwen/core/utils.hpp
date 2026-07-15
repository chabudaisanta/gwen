#pragma once
#include <concepts>

namespace gwen {

/**
 * @brief a の値が b より小さい場合、a を b で更新する
 * @tparam T1 更新対象の型
 * @tparam T2 比較値の型
 * @param a 更新対象の変数
 * @param b 比較する値
 * @return true 更新された場合
 * @return false 更新されなかった場合
 */
template <typename T1, typename T2>
inline bool chmax(T1& a, T2 b) { return (a < b ? a = b, true : false); }

/**
 * @brief a の値が b より大きい場合、a を b で更新する
 * @tparam T1 更新対象の型
 * @tparam T2 比較値の型
 * @param a 更新対象の変数
 * @param b 比較する値
 * @return true 更新された場合
 * @return false 更新されなかった場合
 */
template <typename T1, typename T2>
inline bool chmin(T1& a, T2 b) { return (a > b ? a = b, true : false); }

/**
 * @brief x が半開区間 [l, r) に含まれるか判定する
 * @tparam T 整数型
 * @param x 判定する値
 * @param l 区間の下限 (含む)
 * @param r 区間の上限 (含まない)
 * @return true 区間に含まれる場合
 * @return false 区間に含まれない場合
 */
template <std::integral T>
inline bool isIn(T x, T l, T r) { return (l <= x) && (x < r); }

/**
 * @brief x が半開区間 [l, r) の外側にあるか判定する
 * @tparam T 整数型
 * @param x 判定する値
 * @param l 区間の下限 (含む)
 * @param r 区間の上限 (含まない)
 * @return true 区間の外側にある場合
 * @return false 区間に含まれる場合
 */
template <std::integral T>
inline bool isOut(T x, T l, T r) { return (x < l) || (r <= x); }

/**
 * @brief 2つの半開区間 [l0, r0) と [l1, r1) が重なっているか判定する
 * @tparam T 整数型
 * @param l0 1つ目の区間の下限 (含む)
 * @param r0 1つ目の区間の上限 (含まない)
 * @param l1 2つ目の区間の下限 (含む)
 * @param r1 2つ目の区間の上限 (含まない)
 * @return true 重なっている場合
 * @return false 重なっていない場合
 */
template <std::integral T>
inline bool isOverlap(T l0, T r0, T l1, T r1) { return !(r1 <= l0 || r0 <= l1); }

} // namespace gwen
