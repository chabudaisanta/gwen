#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

template <typename Monoid> struct disjoint_sparse_table {
    using S = typename Monoid::S;
    std::vector<std::vector<S>> data;
    i32 N;

    disjoint_sparse_table() : N(0) {}
    explicit disjoint_sparse_table(const std::vector<S>& vec) : N(vec.size()) {
        i32 b = std::bit_width(vec.size());

        data.assign(b, vec);
        for (i32 i = 1; i < b; ++i) {
            i32 s = 1 << i;
            i32 s2 = s << 1;
            // 正方向累積
            for (i32 l = s; l < N; l += s2) {
                i32 r = std::min(l + s, N);
                for (i32 k = l; k < r - 1; k++) data[i][k + 1] = Monoid::op(data[i][k], data[i][k + 1]);
            }
            // 負方向累積
            for (i32 l = 0; l < N; l += s2) {
                i32 r = std::min(l + s, N);
                for (i32 k = r - 1; k > 0; --k) data[i][k - 1] = Monoid::op(data[i][k - 1], data[i][k]);
            }
        }
    }

    S prod(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= N);
        r--;
        if (r < l) return Monoid::e();
        if (l == r) return data[0][l];

        i32 b = std::bit_width(u32(l ^ r)) - 1;
        return Monoid::op(data[b][l], data[b][r]);
    }
};
}  // namespace gwen