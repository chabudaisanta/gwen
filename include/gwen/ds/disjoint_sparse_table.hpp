#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 静的配列に対する Disjoint Sparse Table
 * @details 結合則を満たす演算で、順序を保った区間積を $O(1)$ で取得します。
 * @tparam M 区間積に使うモノイド
 */
template <monoid M> class DisjointSparseTable {
public:
    using S = typename M::S;

private:
    i32 n;
    std::vector<S> values;
    std::vector<std::vector<S>> table;

public:
    /**
     * @brief 配列から Disjoint Sparse Table を構築する
     * @param initial_values 初期配列
     */
    explicit DisjointSparseTable(const std::vector<S>& initial_values)
        : n(i32(initial_values.size())), values(initial_values) {
        if (n <= 1) return;

        const i32 levels = i32(std::bit_width(static_cast<u32>(n - 1)));
        table.assign(usize(levels), std::vector<S>(usize(n), M::e()));
        for (i32 k = 0; k < levels; ++k) {
            const usize half = usize{1} << k;
            const usize block = half << 1;
            for (usize start = 0; start < usize(n); start += block) {
                const usize middle = std::min(start + half, usize(n));
                const usize end = std::min(start + block, usize(n));

                if (start < middle) {
                    table[usize(k)][middle - 1] = values[middle - 1];
                    for (usize i = middle - 1; i > start; --i) {
                        table[usize(k)][i - 1] = M::op(values[i - 1], table[usize(k)][i]);
                    }
                }
                if (middle < end) {
                    table[usize(k)][middle] = values[middle];
                    for (usize i = middle + 1; i < end; ++i) {
                        table[usize(k)][i] = M::op(table[usize(k)][i - 1], values[i]);
                    }
                }
            }
        }
    }

    /**
     * @brief 配列の長さを返す
     * @return i32 配列の長さ
     */
    i32 size() const { return n; }

    /**
     * @brief 区間 [l, r) の積を返す
     * @param l 区間の左端 (0 <= l <= r)
     * @param r 区間の右端 (l <= r <= n)
     * @return S 区間 [l, r) の積。空区間では M::e()
     */
    S prod(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return M::e();
        if (r - l == 1) return values[usize(l)];

        const i32 k = i32(std::bit_width(static_cast<u32>(l ^ (r - 1))) - 1);
        return M::op(table[usize(k)][usize(l)], table[usize(k)][usize(r - 1)]);
    }
};

}  // namespace gwen
