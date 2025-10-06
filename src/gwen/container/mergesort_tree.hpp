#pragma once

#include <algorithm>
#include <bit>
#include <vector>

#include "gwen/algebra/monoid.hpp"
#include "gwen/container/sorted_treap.hpp"

namespace gwen {

template <monoid M>
class mergesort_tree {
private:
    int n, bc;

    using S = M::S;
    using MULTISET = gwen::
    sorted_treap<M, std::less<S>>
    ;
    M m;
    struct initialize {
        explicit initialize(i32) {
            MULTISET::init(M{}, std::less<S>{});
        }
    } dummy;
    std::vector<MULTISET> d;

public:
    explicit mergesort_tree() {}
    explicit mergesort_tree(const std::vector<S>& vec)
        : n(vec.size()), bc(std::bit_ceil(vec.size())), m(M{}), dummy(0), d(bc * 2) {
        std::vector<std::vector<S>> buf(2 * bc);
        for (i32 i = 0; i < n; ++i) {
            buf[i + bc].emplace_back(vec[i]);
            d[i + bc] = MULTISET(vec[i]);
        }

        for (i32 i = bc - 1; i >= 1; --i) {
            buf[i].resize(buf[i * 2].size() + buf[i * 2 + 1].size());
            std::merge(buf[i * 2].begin(), buf[i * 2].end(),
                       buf[i * 2 + 1].begin(), buf[i * 2 + 1].end(),
                       buf[i].begin());
            d[i] = MULTISET(buf[i]);
        }
    }

    i32 lower_cnt(i32 l, i32 r, const S& x) {
        assert(0 <= l && l <= r && r <= n && "lower_cnt");
        l += bc;
        r += bc;
        i32 ret = 0;

        while (l < r) {
            if (l & 1) {
                ret += d[l].lower_bound_pos(x);
                l++;
            }
            if (r & 1) {
                --r;
                ret += d[r].lower_bound_pos(x);
            }
            l >>= 1;
            r >>= 1;
        }
        return ret;
    }

    S lower_prod(i32 l, i32 r, const S& x) {
        assert(0 <= l && l <= r && r <= n && "lower_prod");
        l += bc;
        r += bc;
        S sml = m.e, smr = m.e;

        while (l < r) {
            if (l & 1) {
                sml = m.op(sml, d[l].split_prod_lower(x).first);
                l++;
            }
            if (r & 1) {
                --r;
                smr = m.op(d[r].split_prod_lower(x).first, smr);
            }
            l >>= 1;
            r >>= 1;
        }
        return m.op(sml, smr);
    }

    S upper_prod(i32 l, i32 r, const S& x) {
        assert(0 <= l && l <= r && r <= n && "lower_prod");
        l += bc;
        r += bc;
        S sml = m.e, smr = m.e;

        while (l < r) {
            if (l & 1) {
                sml = m.op(sml, d[l].split_prod_upper(x).first);
                l++;
            }
            if (r & 1) {
                --r;
                smr = m.op(d[r].split_prod_upper(x).first, smr);
            }
            l >>= 1;
            r >>= 1;
        }
        return m.op(sml, smr);
    }

    void set(i32 p, const S& x) {
        assert(0 <= p && p <= n);
        const S& prv = d[p + bc].at(0);
        for (i32 i = p + bc; i >= 1; i >>= 1) {
            d[i].erase(prv);
            d[i].insert(x);
        }
    }
};

}  // namespace gwen