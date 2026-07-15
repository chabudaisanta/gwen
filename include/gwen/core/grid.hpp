#pragma once
#include <utility>
#include <cassert>

namespace gwen {

/** @brief 2次元グリッド上の8方向＋停止を表す座標増分配列 (y, x) のペア */
constexpr std::pair<int, int> mv[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 0}};
/** @brief 2次元グリッド上の8方向＋停止を表すX方向（幅）の増分配列 */
constexpr int dw[] = {0, 0, -1, 1, -1, -1, 1, 1, 0};
/** @brief 2次元グリッド上の8方向＋停止を表すY方向（高さ）の増分配列 */
constexpr int dh[] = {-1, 1, 0, 0, -1, 1, -1, 1, 0};

/**
 * @brief 指定した座標から特定の方向へ移動した後の座標を返す
 * @param hi 現在のY座標（高さ）
 * @param wi 現在のX座標（幅）
 * @param dir 移動方向のインデックス (0 <= dir < 9)
 * @return std::pair<int, int> 移動後の (Y, X) 座標
 */
inline std::pair<int, int> mv_to(int hi, int wi, int dir) {
    assert(0 <= dir && dir < 9);
    return std::make_pair(hi + dh[dir], wi + dw[dir]);
}

} // namespace gwen
