#line 2 "include/gwen/container/segtree.hpp"

// TODO : ACL依存をなくすために書く。優先度低
#include <atcoder/lazysegtree.hpp>
#include <atcoder/segtree.hpp>
namespace gwen {
template <typename Monoid> using segtree = atcoder::segtree<typename Monoid::S, Monoid::op, Monoid::e>;
template <typename ActedMonoid>
using lazysegtree = atcoder::lazy_segtree<typename ActedMonoid::S,
                                          ActedMonoid::op,
                                          ActedMonoid::e,
                                          typename ActedMonoid::F,
                                          ActedMonoid::mapping,
                                          ActedMonoid::composition,
                                          ActedMonoid::id>;
}  // namespace gwen
