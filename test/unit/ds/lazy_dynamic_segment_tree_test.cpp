// clang-format off
#include "gwen/ds/lazy_dynamic_segment_tree.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;

using mint = DynamicModInt64;
using Monoid = range_affine_range_sum_monoid<mint>;

TEST(LazyDynamicSegmentTreeTest, RangeAffineRangeSum) {
    mint::set_mod(998244353);
    i64 n = 1000000000000LL;
    LazyDynamicSegmentTree<Monoid> seg(n);

    seg.set(100, Monoid::unit(10));
    seg.set(1000, Monoid::unit(20));

    EXPECT_EQ(seg.prod(0, n).val.val(), 30);
    EXPECT_EQ(seg.prod(100, 1001).val.val(), 30);
    EXPECT_EQ(seg.prod(100, 1000).val.val(), 10);

    // apply x -> 2x + 1 to [50, 500)
    // the node at 100 becomes 10 * 2 + 1 = 21
    seg.apply(50, 500, {mint(2), mint(1)});

    EXPECT_EQ(seg.get(100).val.val(), 21);
    EXPECT_EQ(seg.get(100).len.val(), 1);

    EXPECT_EQ(seg.get(1000).val.val(), 20);  // unchanged

    EXPECT_EQ(seg.prod(0, n).val.val(), 41);
}
