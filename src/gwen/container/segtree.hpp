#pragma once

// TODO : ACL依存をなくすために書く。優先度低
#include <atcoder/lazysegtree>
#include <atcoder/segtree>
namespace gwen {
template <typename Monoid> using segtree = atcoder::segtree<typename Monoid::S, Monoid::op, Monoid::e>;
template <typename ActedMonoid>
using lazysegtree = atcoder::lazysegtree<typename ActedMonoid::S,
                                         ActedMonoid::op,
                                         ActedMonoid::e,
                                         typename ActedMonoid::F,
                                         ActedMonoid::mapping,
                                         ActedMonoid::composition,
                                         ActedMonoid::id>;
}  // namespace gwen