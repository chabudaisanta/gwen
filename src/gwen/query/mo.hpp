#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <numeric>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {
namespace internal {
// https://take44444.github.io/Algorithm-Book/range/mo/main.html
i64 hilbert_order(i64 x, i64 y, u64 n) {
    assert(std::has_single_bit(n));
    i64 rx, ry, d = 0;
    for (i64 s = n >> 1; s; s >>= 1) {
        rx = (x & s) > 0, ry = (y & s) > 0;
        d += s * s * ((rx * 3) ^ ry);
        if (ry) continue;
        if (rx) {
            x = n - 1 - x;
            y = n - 1 - y;
        }
        std::swap(x, y);
    }
    return d;
}

}  // namespace internal

class mo_algorithm {
private:
    i32 q = 0;
    i32 n, bc;
    std::vector<i32> L, R;
    std::vector<i64> ord;

public:
    explicit mo_algorithm(i32 n_) : n(n_), bc(std::bit_ceil(static_cast<u32>(n))) {}

    void add_query(i32 l, i32 r) {
        // [l, r)
        // assert(0 <= l && l <= r && r <= n);
        L.emplace_back(l);
        R.emplace_back(r);
        ord.emplace_back(internal::hilbert_order(l, r, bc));
        q++;
    }

    auto solve(auto&& increment_l, auto&& decrement_l, auto&& increment_r, auto&& decrement_r, auto&& get_res) -> std::vector<decltype(get_res())> {
        std::vector<i32> query(q);
        std::iota(query.begin(), query.end(), 0);
        std::sort(query.begin(), query.end(), [&](i32 a, i32 b) {
            return ord[a] < ord[b];
        });

        using S = decltype(get_res());
        std::vector<S> ret(q);
        i32 l = 0, r = 0;
        for (i32 idx : query) {
            i32 nl = L[idx], nr = R[idx];

            // 先に拡張
            while (nl < l) decrement_l(--l, r);
            while (r < nr) increment_r(l, r++);
            // 後から縮小
            while (l < nl) increment_l(l++, r);
            while (nr < r) decrement_r(l, --r);

            ret[idx] = get_res();
        }
        return ret;
    }
};

}  // namespace gwen