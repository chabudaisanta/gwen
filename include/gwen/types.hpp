#pragma once

#include <cstddef>
#include <cstdint>

namespace gwen {

/** @name 符号あり整数型 */
///@{
using i32 = std::int32_t;
using i64 = std::int64_t;
using i128 = __int128_t;
///@}

/** @name 符号なし整数型 */
///@{
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using u128 = __uint128_t;
///@}

/** @name ポインタサイズ・コンテナサイズ型 */
///@{
using usize = std::size_t;
using isize = std::ptrdiff_t;
///@}

/** @name 浮動小数点型 */
///@{
using f32 = float;
using f64 = double;
using f80 = long double;
///@}

/**
 * @brief 数値リテラル用のユーザー定義リテラル
 */
namespace literals {

/** @brief i64 型のリテラル */
constexpr i64 operator""_i64(unsigned long long n) { return static_cast<i64>(n); }
/** @brief u64 型のリテラル */
constexpr u64 operator""_u64(unsigned long long n) { return static_cast<u64>(n); }
/** @brief usize 型のリテラル */
constexpr usize operator""_zu(unsigned long long n) { return static_cast<usize>(n); }

}  // namespace literals

}  // namespace gwen
