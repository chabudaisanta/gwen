#pragma once
#include <cassert>
#include <vector>

#include "gwen/container/fenwick_tree.hpp"
#include "gwen/types.hpp"
namespace gwen {

template <typename Abel> struct range_add_point_get {
    using S = typename Abel::S;
    i32 N;
    fenwick_tree<Abel> D;

    range_add_point_get() : N(0), D() {}
    explicit range_add_point_get(i32 N_) : N(N_), D(N_) {}
    explicit range_add_point_get(const std::vector<S>& vec) : N(vec.size()) {
        // D[i] := -A[i-1] + D[i] (A[-1] = 0)
        // D[i] := Abel::op(Abel::inv(i == 0 ? Abel::e() : A[i-1]), A[i])
        S prv = Abel::e();
        std::vector<S> tmp(N);
        for (i32 i = 0; i < N; ++i) {
            tmp[i] = Abel::op(Abel::inv(prv), vec[i]);
        }
        D = fenwick_tree<Abel>(tmp);
    }

    void add(i32 p, S x) {
        assert(0 <= p && p < N);
        D.add(p, x);
        if (p < N - 1) D.add(p + 1, Abel::inv(x));
    }
    void add(i32 l, i32 r, S x) {
        assert(0 <= l && l < r && r <= N);
        D.add(l, x);
        D.add(r, Abel::inv(x));
    }
    S get(i32 p) const {
        assert(0 <= p && p < N);
        return D.sum(p + 1);
    }
};
}  // namespace gwen