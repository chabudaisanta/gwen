#pragma once

#include <cassert>
#include <concepts>
#include <ranges>
#include <vector>

#include "gwen/algebra/monoid.hpp"
#include "gwen/misc/xorshift.hpp"
#include "gwen/types.hpp"

namespace gwen {

namespace internal {

template <monoid M, typename Compare>
class sorted_treap_impl {
public:
    using S = M::S;
    using tree = in;

private:
    struct node {
        tree lch;
        tree rch;
        S val;
        S prod;
        int cnt;
        u32 pri;
    };

    std::vector<node> d;
    static constexpr tree NIL = 0;
    M m;
    Compare comp;
    bool equal(const S& a, const S& b) const {
        return !comp(a, b) && !comp(b, a);
    }
};

} // namespace internal

template <monoid M, typename Compare>
    requires requires(const Compare& comp,
                      const typename M::S& a,
                      const typename M::S& b) {
        { comp(a, b) } -> std::same_as<bool>;
    }
class sorted_treap {
public:
    static void init(const M& m, const Compare& comp) {}

private:
    using internal::sorted_treap<M,Compare>::tree;
    int id;

};

}  // namespace gwen