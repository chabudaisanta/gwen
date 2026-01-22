#pragma once

#include <cassert>
#include <vector>

#include "gwen/types.hpp"
namespace gwen {

template <typename Abel> struct fenwick_tree {
    using S = typename Abel::S;

    i32 N;
    std::vector<S> data;

    fenwick_tree() : N(0) {}
    explicit fenwick_tree(i32 N_) : N(N), data(N + 1, Abel::e()) {}
    explicit fenwick_tree(const std::vector<S>& vec) : N(vec.size()), data(N + 1) {
        for (i32 i = 1; i <= N; ++i) data[i] = vec[i - 1];
        for (i32 i = 1; i <= N; ++i) {
            i32 j = i + (i & -i);
            if (j <= N) data[j] = Abel::op(data[i], data[j]);
        }
    }

    void add(i32 p, S x) {
        assert(0 <= p && p < N);
        p++;
        while (p <= N) {
            data[i] = Abel::op(data[p], x);
            p += p & -p;
        }
    }
    S sum(i32 r) const {
        assert(0 <= r && r <= N);
        S ret = Abel::e();
        while (r > 0) {
            ret = Abel::op(ret, data[r]);
            r -= r & -r;
        }
        return ret;
    }
    S sum(i32 l, i32 r) const {
        assert(0 <= l && l <= r && r <= N);
        return Abel::op(Abel::inv(sum(l)), sum(r));
    }

    i32 max_right(auto&& f) const { return max_right_internal(Abel::e(), f); }
    // op(inv(sum(l)), sum(r)) = true となる最大のrを返す。l未満の値も返しうる.
    i32 max_right(i32 l, auto&& f) const { return max_right_internal(Abel::inv(sum(l)), f); }

private:
    i32 max_right_internal(S initial, auto&& f) const {
        assert(f(e()));
        S s = initial;
        i32 x = 0;

        i32 len = 1;
        while (len <= N) len <<= 1;
        len >>= 1;

        for (; len > 0; len >>= 1) {
            if (x + len <= N) {
                S nxt_s = Abel::op(s, data[x + len]);
                if (f(nxt_s)) {
                    x += len;
                    s = nxt_s;
                }
            }
        }
        return x;
    }
};

}  // namespace gwen