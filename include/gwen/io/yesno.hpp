#pragma once
#include <iostream>

namespace gwen {

/**
 * @brief 真偽値に応じて "Yes" または "No" を改行付きで出力する
 * @param b 判定する真偽値（true なら "Yes", false なら "No"）
 */
inline void yon(bool b) { std::cout << (b ? "Yes\n" : "No\n"); }

}  // namespace gwen
