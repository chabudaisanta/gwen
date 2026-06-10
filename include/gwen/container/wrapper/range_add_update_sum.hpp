#pragma once

#include <atcoder/lazysegtree>
#include <vector>

#include "gwen/algebra/range_affine_range_sum_monoid.hpp"
#include "gwen/types.hpp"

namespace gwen {

template <typename T> struct range_add_update_sum {
    using M = range_affine_range_sum_monoid<T>;
    using Segtree = atcoder::lazy_segtree<typename M::S, M::op, M::e, typename M::F, M::mapping, M::composition, M::id>;

    i32 N;
    Segtree seg;

    explicit range_add_update_sum(const std::vector<T>& vec) : N(vec.size()) {
        std::vector<typename M::S> tmp(N);
        for (i32 i = 0; i < N; ++i) tmp[i] = {vec[i], 1};
        seg = Segtree(tmp);
    }

    void add(i32 l, i32 r, T x) { seg.apply(l, r, {1, x}); }
    void add(i32 p, T x) { seg.apply(p, {1, x}); }

    void update(i32 l, i32 r, T x) { seg.apply(l, r, {0, x}); }
    void update(i32 p, T x) { seg.apply(p, {0, x}); }

    void multiply(i32 l, i32 r, T x) { seg.apply(l, r, {x, 0}); }
    void multiply(i32 p, T x) { seg.apply(p, {x, 0}); }

    void affine(i32 l, i32 r, T a, T b) { seg.apply(l, r, {a, b}); }
    void affine(i32 p, T a, T b) { seg.apply(p, {a, b}); }

    T all_prod() { return seg.all_prod().val; }
    T prod(i32 l, i32 r) { return seg.prod(l, r).val; }
    T get(i32 p) { return seg.get(p).val; }
};

}  // namespace gwen