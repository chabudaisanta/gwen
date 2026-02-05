#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "gwen/types.hpp"

namespace gwen {

template <typename Abel>
struct weighted_dsu {
    using S = typename Abel::S;

    i32 n;
    std::vector<i32> parent_or_size;
    std::vector<S> pot;

    weighted_dsu() : n(0) {}
    explicit weighted_dsu(i32 n_) : n(n_), parent_or_size(n_, -1), pot(n_, Abel::e()) {}

    i32 leader(i32 a) {
        assert(0 <= a && a < n);
        if (parent_or_size[a] < 0) return a;
        i32 r = leader(parent_or_size[a]);
        pot[a] = Abel::op(pot[a], pot[parent_or_size[a]]);
        return parent_or_size[a] = r;
    }

    S diff(i32 a, i32 b) {
        assert(0 <= a && a < n);
        assert(0 <= b && b < n);
        return Abel::op(potential(a), Abel::inv(potential(b)));
    }

    bool same(i32 a, i32 b) {
        assert(0 <= a && a < n);
        assert(0 <= b && b < n);
        return leader(a) == leader(b);
    }

    // P(a) - P(leader(a))
    S potential(i32 a) {
        leader(a);
        return pot[a];
    }

    // merge so that diff(a, b) = w (i.e. P(a) - P(b) = w)
    // returns new leader on success, -1 if constraint is inconsistent (a,b already in same component)
    i32 merge(i32 a, i32 b, S w) {
        assert(0 <= a && a < n);
        assert(0 <= b && b < n);
        w = Abel::op(w, Abel::op(potential(b), Abel::inv(potential(a))));
        a = leader(a);
        b = leader(b);
        if (a == b) {
            return (w == Abel::e()) ? a : -1;
        }
        if (-parent_or_size[a] < -parent_or_size[b]) {
            std::swap(a, b);
            w = Abel::inv(w);
        }
        parent_or_size[a] += parent_or_size[b];
        parent_or_size[b] = static_cast<i32>(a);
        pot[b] = Abel::inv(w);  // P(b) - P(a) = -w = inv(w)
        return a;
    }

    i32 size(i32 a) {
        assert(0 <= a && a < n);
        return -parent_or_size[leader(a)];
    }

    std::vector<std::vector<i32>> groups() {
        std::vector<i32> leader_buf(n);
        std::vector<i32> group_size(n, 0);
        for (i32 i = 0; i < n; ++i) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<i32>> result(n);
        for (i32 i = 0; i < n; ++i) result[i].reserve(group_size[i]);
        for (i32 i = 0; i < n; ++i) result[leader_buf[i]].push_back(i);
        result.erase(
            std::remove_if(result.begin(), result.end(),
                           [](const std::vector<i32>& v) { return v.empty(); }),
            result.end());
        return result;
    }
};

}  // namespace gwen
