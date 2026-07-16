#pragma once

#include <cassert>
#include <concepts>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief カウンティングソート (Counting Sort) による安定ソートインデックス配列の生成
 * @details 値域が [0, M] である整数列 A に対して、安定ソートされた順列（インデックスの配列）を返します。
 * 計算量は O(N + M) です。
 *
 * 返される配列 P は、0 <= l < r < N を満たす (l, r) の組に対して、
 * - P[l] < P[r] であるときには A[P[l]] <= A[P[r]]
 * - P[l] > P[r] であるときには A[P[l]] > A[P[r]]
 * を満たす安定ソートされた順序を表します。
 *
 * @tparam T 配列の要素の型 (整数型)
 * @param A ソート対象の配列
 * @param M 配列 A の要素の最大値 (0 <= A[i] <= M)
 * @return std::vector<i32> ソートされたインデックス配列
 */
template <std::integral T> std::vector<i32> counting_sort(const std::vector<T>& a, i32 m) {
    std::vector<i32> s(m + 2, 0);
    i32 n = static_cast<i32>(a.size());
    for (i32 i = 0; i < n; ++i) {
        assert(0 <= a[i] && a[i] <= m);
        s[a[i] + 1]++;
    }
    for (i32 j = 0; j <= m; ++j) {
        s[j + 1] += s[j];
    }
    std::vector<i32> ret(n);
    for (i32 i = 0; i < n; ++i) {
        ret[s[a[i]]++] = i;
    }
    return ret;
}

}  // namespace gwen
