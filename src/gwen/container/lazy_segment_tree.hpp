#pragma once

#include <bit>
#include <cassert>
#include <vector>

#include "gwen/types.hpp"
#include "gwen/algebra/monoid.hpp"

namespace gwen {

template<monoid M, monoid A, class Mapping>
requires (const typename A::S& f, const typename M::S& x) {
    { Mapping::op(f, x) } -> std::same_as<typename typename M::S>;
}
class lazy_segment_tree {
private:
    using S = M::S;
    using F = A::S;
    i32 n, bc, lg;
    std::vector<S> d, lz;

public:
    explicit lazy_segment_tree() : lazy_segment_tree(0) {}
    explicit lazy_segment_tree(i32 n_) : lazy_segment_tree(std::vector<S>(n_, M::e)) {}
    explicit lazy_segment_tree(const std::vector<S>& vec) : n(vec.size()), bc(std::bit_ceil(vec.size())),
    lg(n == 0 ? 0 : std::bit_width(vec.size() - 1)) {
        d.assign(bc * 2, M::e);
        lz.assign(bc * 2, A::e);
        for(i32 i = 0; i < n; ++i) d[i + bc] = vec[i];
        for(i32 i = bc - 1; i >= 1; --i) {
            d[i] = M::op(d[i * 2], d[i * 2 + 1]);
        }
    }

    void set(i32 p, const S& x) {
        assert(0 <= p && p < n);
        eval(p);
        d[p + bc] = x;
    }
    void get(i32 p) {
        assert(0 <= p && p < n);
        return eval(p);
    }

private:
    S eval(i32 p) {
        i32 tgt = p + bc;
        for(i32 i = lg; i >= 0; --i) {
            i32 cur = tgt >> i;
            d[cur] = Mapping::op(lz[cur], d[cur]);
            if(i > 0) {
                lz[cur * 2] = A::op(lz[cur * 2], lz[cur]);
                lz[cur * 2 + 1] = A::op(lz[cur * 2 + 1], lz[cur]);
            }
            lz[cur] = A::e;
        }
        return d[tgt];
    }
};

}