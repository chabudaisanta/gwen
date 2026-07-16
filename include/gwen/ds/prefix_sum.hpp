#pragma once
#include <cassert>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 1次元配列の累積和を構築する
 * @tparam T 要素の型
 * @param vec 元の配列
 * @return std::vector<T> サイズが元の配列より 1 大きい累積和の配列
 */
template <typename T> std::vector<T> prefix_sum(const std::vector<T>& vec) {
    i32 N = vec.size();
    std::vector<T> PS(N + 1);
    for (i32 i = 0; i < N; ++i) PS[i + 1] = PS[i] + vec[i];
    return PS;
}

/**
 * @brief 2次元グリッドの累積和を管理する構造体
 * @tparam T 要素の型
 */
template <typename T> struct PrefixSum2d {
    std::vector<std::vector<T>> dat;
    i32 H, W;

    /**
     * @brief 2次元配列から累積和を構築する
     * @param vec 元の2次元配列
     */
    explicit PrefixSum2d(const std::vector<std::vector<T>>& vec) : H(vec.size()), W(H > 0 ? vec[0].size() : 0) {
        dat.resize(H + 1, std::vector<T>(W + 1, T{}));
        for (i32 i = 0; i < H; ++i)
            for (i32 j = 0; j < W; ++j) dat[i + 1][j + 1] = vec[i][j];
        for (i32 i = 0; i < H; ++i)
            for (i32 j = 0; j < W; ++j) dat[i + 1][j + 1] += dat[i][j + 1];
        for (i32 i = 0; i < H; ++i)
            for (i32 j = 0; j < W; ++j) dat[i + 1][j + 1] += dat[i + 1][j];
    }

    /**
     * @brief 半開区間 [u, d) x [l, r) の矩形領域の和を取得する
     * @param l 左端の列インデックス (含む)
     * @param r 右端の列インデックス (含まない)
     * @param u 上端の行インデックス (含む)
     * @param d 下端の行インデックス (含まない)
     * @return T 矩形領域の要素の総和
     */
    T get(i32 l, i32 r, i32 u, i32 d) const {
        assert(0 <= l && l <= r && r <= W);
        assert(0 <= u && u <= d && d <= H);
        return dat[d][r] - dat[d][l] - dat[u][r] + dat[u][l];
    }
};

}  // namespace gwen
