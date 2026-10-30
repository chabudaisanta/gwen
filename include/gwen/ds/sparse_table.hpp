#pragma once

#include <bit>
#include <cassert>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

namespace gwen {

/**
 * @brief 静的配列に対する Sparse Table
 * @details 区間積を $O(1)$ で取得します。演算の冪等性が必要です。
 * @tparam M 区間積に使うモノイド
 */
template <monoid M> class SparseTable {
public:
    using S = typename M::S;

private:
    i32 n;
    std::vector<std::vector<S>> table;

public:
    /**
     * @brief 配列から Sparse Table を構築する
     * @param values 初期配列
     */
    explicit SparseTable(const std::vector<S>& values) : n(i32(values.size())) {
        if (n == 0) return;

        const i32 levels = i32(std::bit_width(static_cast<u32>(n)));
        table.reserve(usize(levels));
        table.push_back(values);
        for (i32 k = 1; k < levels; ++k) {
            const i32 half = i32{1} << (k - 1);
            const i32 length = i32{1} << k;
            const i32 count = n - length + 1;
            table.emplace_back(usize(count));
            for (i32 i = 0; i < count; ++i) {
                table.back()[usize(i)] = M::op(table[usize(k - 1)][usize(i)], table[usize(k - 1)][usize(i + half)]);
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

        const i32 k = i32(std::bit_width(static_cast<u32>(r - l)) - 1);
        const i32 length = i32{1} << k;
        return M::op(table[usize(k)][usize(l)], table[usize(k)][usize(r - length)]);
    }
};

}  // namespace gwen
