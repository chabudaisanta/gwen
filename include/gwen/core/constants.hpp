#pragma once
#include "gwen/types.hpp"

namespace gwen {

/** @brief 頻出の素数モジュロ (998244353) */
constexpr i32 mod998 = 998244353;
/** @brief 頻出の素数モジュロ (1000000007) */
constexpr i32 mod107 = 1000000007;
/** @brief 頻出の素数モジュロ (1000000009) */
constexpr i32 mod109 = 1000000009;
/** @brief 頻出の素数モジュロ (2147483647) */
constexpr i32 mod31 = 2147483647;
/** @brief ローリングハッシュ等で使われる素数モジュロ ((1<<61)-1) */
constexpr i64 mod61 = (1LL << 61) - 1;

/** @brief int型の十分大きな値 (1001001001) */
constexpr i32 iINF = 1001001001;
/** @brief i64型の十分大きな値 ((1LL<<60)+1) */
constexpr i64 liINF = (1LL << 60) + 1;

/** @brief 改行文字 */
constexpr char EL = '\n';
/** @brief 空白文字 */
constexpr char SPA = ' ';

}  // namespace gwen
