// clang-format off
#include "gwen/alge/range_affine_range_sum_monoid.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

using namespace gwen;

using mint = DynamicModInt64;
using Monoid = range_affine_range_sum_monoid<mint>;
using S = Monoid::S;
using F = Monoid::F;

TEST(RangeAffineRangeSumMonoidTest, BasicOperations) {
    mint::set_mod(998244353);

    S a = Monoid::unit(3); // {3, 1}
    S b = Monoid::unit(5); // {5, 1}

    // op
    S c = Monoid::op(a, b);
    EXPECT_EQ(c.val.val(), 8);
    EXPECT_EQ(c.len.val(), 2);

    // e
    S e = Monoid::e();
    EXPECT_EQ(e.val.val(), 0);
    EXPECT_EQ(e.len.val(), 0);
    
    S c_e = Monoid::op(c, e);
    EXPECT_EQ(c_e.val.val(), c.val.val());
    EXPECT_EQ(c_e.len.val(), c.len.val());

    // mapping (f(x) = 2x + 3)
    F f = {2, 3};
    S mapped_c = Monoid::mapping(f, c);
    // val = 2 * 8 + 3 * 2 = 16 + 6 = 22
    EXPECT_EQ(mapped_c.val.val(), 22);
    EXPECT_EQ(mapped_c.len.val(), 2);

    // composition (g(x) = 4x + 5)
    F g = {4, 5};
    // f(g(x)) = 2(4x + 5) + 3 = 8x + 13
    F comp = Monoid::composition(f, g);
    EXPECT_EQ(comp.a.val(), 8);
    EXPECT_EQ(comp.b.val(), 13);

    // id
    F id = Monoid::id();
    EXPECT_EQ(id.a.val(), 1);
    EXPECT_EQ(id.b.val(), 0);

    F comp_id = Monoid::composition(f, id);
    EXPECT_EQ(comp_id.a.val(), f.a.val());
    EXPECT_EQ(comp_id.b.val(), f.b.val());
}
