// clang-format off
#include "gwen/bbst/lazy_sorted_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;
using mint = DynamicModInt64;
using Monoid = range_affine_range_sum_monoid<mint>;

TEST(LazySortedTreapTest, BasicOperations) {
    mint::set_mod(998244353);
    LazySortedTreap<int, Monoid> t;

    // insert (key, value). value is a struct {mint a, i32 b}, size is b
    t.insert(10, {mint(5), 1});   // 10 -> 5
    t.insert(5, {mint(2), 1});    // 5 -> 2
    t.insert(20, {mint(10), 1});  // 20 -> 10

    EXPECT_EQ(t.size(), 3);

    // prod(0, 15) should cover 5 and 10. sum = 2 + 5 = 7
    auto p1 = t.prod(0, 15);
    EXPECT_EQ(p1.val.val(), 7);
    EXPECT_EQ(p1.len.val(), 2);

    // Apply affine x -> 2x + 3 to range [0, 15)
    // 5 -> 2*2 + 3 = 7
    // 10 -> 5*2 + 3 = 13
    // 20 -> 10 (unchanged)
    // F is struct {mint a, b}
    t.apply(0, 15, {mint(2), mint(3)});

    auto p2 = t.prod(0, 15);
    EXPECT_EQ(p2.val.val(), 20);  // 7 + 13
    EXPECT_EQ(p2.len.val(), 2);

    auto p_all = t.all_prod();
    EXPECT_EQ(p_all.val.val(), 30);  // 7 + 13 + 10
    EXPECT_EQ(p_all.len.val(), 3);

    auto it_begin = t.begin();
    EXPECT_NE(it_begin, t.end());
    EXPECT_EQ(*it_begin, 5);

    auto lb20 = t.lower_bound(20);
    EXPECT_EQ(*lb20, 20);

    auto ub5 = t.upper_bound(5);
    EXPECT_EQ(*ub5, 10);

    auto it_end = t.end();
    --it_end;
    EXPECT_EQ(*it_end, 20);
}
