// clang-format off
#include "gwen/ds/lazy_segment_tree.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;

using mint = DynamicModInt64;
using Monoid = range_affine_range_sum_monoid<mint>;

TEST(LazySegmentTreeTest, RangeAffineRangeSum) {
    mint::set_mod(998244353);
    std::vector<Monoid::S> v;
    for (int i = 1; i <= 5; ++i) v.push_back(Monoid::unit(i));

    LazySegmentTree<Monoid> seg(v);

    EXPECT_EQ(seg.prod(0, 5).val.val(), 15);
    EXPECT_EQ(seg.prod(1, 4).val.val(), 9);

    // apply x -> 2x + 1 to [1, 4)
    // original: 1, 2, 3, 4, 5
    // new: 1, 5, 7, 9, 5
    seg.apply(1, 4, {mint(2), mint(1)});

    EXPECT_EQ(seg.get(0).val.val(), 1);
    EXPECT_EQ(seg.get(1).val.val(), 5);
    EXPECT_EQ(seg.get(2).val.val(), 7);
    EXPECT_EQ(seg.get(3).val.val(), 9);
    EXPECT_EQ(seg.get(4).val.val(), 5);

    EXPECT_EQ(seg.prod(1, 4).val.val(), 21);

    seg.set(2, Monoid::unit(10));
    EXPECT_EQ(seg.get(2).val.val(), 10);
    EXPECT_EQ(seg.prod(0, 5).val.val(), 30);
}
