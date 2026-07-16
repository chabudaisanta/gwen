#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"

#include <iostream>
#include <vector>

#include "gwen/alge/abel.hpp"
#include "gwen/ds/fenwick_tree.hpp"
#include "gwen/query/mo.hpp"
#include "gwen/types.hpp"
#include "gwen/utils/compress.hpp"

using namespace gwen;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, q;
    std::cin >> n >> q;

    std::vector<i32> a(n);
    for (i32 i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    auto comp = compress(a);
    i32 comp_max = 0;
    for (i32 x : comp) {
        if (x > comp_max) comp_max = x;
    }

    Mo mo(n);
    for (i32 i = 0; i < q; ++i) {
        i32 l, r;
        std::cin >> l >> r;
        mo.add_query(l, r);
    }

    FenwickTree<sum_abel<i32>> bit(comp_max + 1);
    i64 current_inv = 0;

    auto increment_l = [&](i32 l, i32 r) {
        i32 v = comp[l];
        current_inv -= bit.sum(0, v);
        bit.add(v, -1);
    };
    auto decrement_l = [&](i32 l, i32 r) {
        i32 v = comp[l];
        current_inv += bit.sum(0, v);
        bit.add(v, 1);
    };
    auto increment_r = [&](i32 l, i32 r) {
        i32 v = comp[r];
        current_inv += bit.sum(v + 1, comp_max + 1);
        bit.add(v, 1);
    };
    auto decrement_r = [&](i32 l, i32 r) {
        i32 v = comp[r];
        current_inv -= bit.sum(v + 1, comp_max + 1);
        bit.add(v, -1);
    };
    auto get_res = [&](i32 idx) { return current_inv; };

    auto res = mo.solve(increment_l, decrement_l, increment_r, decrement_r, get_res);

    for (i32 i = 0; i < q; ++i) {
        std::cout << res[i] << "\n";
    }

    return 0;
}
