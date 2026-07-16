#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <limits>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 最長増加部分列 (Longest Increasing Subsequence)
 * @details 与えられた配列の最長増加部分列 (単調非減少ではない、真に増加する部分列) を1つ構築して返します。
 *
 * @tparam T 配列の要素の型 (順序付け可能であること)
 * @param vec 入力配列
 * @return std::vector<T> 最長増加部分列
 */
template <std::totally_ordered T> std::vector<T> lis_strict(const std::vector<T>& vec) {
    i32 n = static_cast<i32>(vec.size());
    if (n == 0) return {};

    // DP テーブル: dp[i] は長さ i+1 の増加部分列の末尾の最小値
    std::vector<T> dp;
    // id[i] は dp[i] に格納されている値の元の vec におけるインデックス
    std::vector<i32> id;
    // prv[i] は vec[i] を末尾とする最長増加部分列の直前の要素のインデックス
    std::vector<i32> prv(n, -1);

    for (i32 i = 0; i < n; ++i) {
        auto it = std::lower_bound(dp.begin(), dp.end(), vec[i]);
        i32 p = static_cast<i32>(std::distance(dp.begin(), it));

        prv[i] = (p == 0 ? -1 : id[p - 1]);

        if (it == dp.end()) {
            dp.push_back(vec[i]);
            id.push_back(i);
        }
        else {
            *it = vec[i];
            id[p] = i;
        }
    }

    std::vector<T> ret;
    i32 cur = id.back();
    while (cur != -1) {
        ret.push_back(vec[cur]);
        cur = prv[cur];
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}

/**
 * @brief 最長非減少部分列 (Longest Non-decreasing Subsequence)
 * @details 与えられた配列の最長非減少部分列 (単調非減少な部分列) を1つ構築して返します。
 * 旧来の `LIS` に対応します。
 *
 * @tparam T 配列の要素の型 (順序付け可能であること)
 * @param vec 入力配列
 * @return std::vector<T> 最長非減少部分列
 */
template <std::totally_ordered T> std::vector<T> lis(const std::vector<T>& vec) {
    i32 n = static_cast<i32>(vec.size());
    if (n == 0) return {};

    std::vector<T> dp;
    std::vector<i32> id;
    std::vector<i32> prv(n, -1);

    for (i32 i = 0; i < n; ++i) {
        auto it = std::upper_bound(dp.begin(), dp.end(), vec[i]);
        i32 p = static_cast<i32>(std::distance(dp.begin(), it));

        prv[i] = (p == 0 ? -1 : id[p - 1]);

        if (it == dp.end()) {
            dp.push_back(vec[i]);
            id.push_back(i);
        }
        else {
            *it = vec[i];
            id[p] = i;
        }
    }

    std::vector<T> ret;
    i32 cur = id.back();
    while (cur != -1) {
        ret.push_back(vec[cur]);
        cur = prv[cur];
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
}

}  // namespace gwen
