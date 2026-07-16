// clang-format off
#include "gwen/bbst/lazy_implicit_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/range_affine_range_sum_monoid.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;
using mint = DynamicModInt64;
using Monoid = range_affine_range_sum_monoid<mint>;

TEST(LazyImplicitTreapTest, BasicOperations) {
    mint::set_mod(998244353);
    LazyImplicitTreap<Monoid> t;

    t.push_back({mint(1), 1}); // 1
    t.push_back({mint(2), 1}); // 2
    t.push_back({mint(3), 1}); // 3
    t.push_back({mint(4), 1}); // 4
    t.push_back({mint(5), 1}); // 5

    EXPECT_EQ(t.size(), 5);

    // apply x -> 2x + 10 to [1, 4) i.e. elements {2, 3, 4}
    // new elements: 1, 14, 16, 18, 5
    t.apply(1, 4, {mint(2), mint(10)});

    auto p1 = t.prod(1, 4); // 14 + 16 + 18 = 48
    EXPECT_EQ(p1.val.val(), 48);
    EXPECT_EQ(p1.len.val(), 3);

    auto p2 = t.get(2); // 16
    EXPECT_EQ(p2.val.val(), 16);
    EXPECT_EQ(p2.len.val(), 1);

    // reverse [0, 3) i.e. elements {1, 14, 16} -> {16, 14, 1}
    // array is now {16, 14, 1, 18, 5}
    t.reverse(0, 3);

    auto p3 = t.get(0);
    EXPECT_EQ(p3.val.val(), 16);

    auto p_all = t.all_prod(); // 16 + 14 + 1 + 18 + 5 = 54
    EXPECT_EQ(p_all.val.val(), 54);

    t.erase(2); // remove element at index 2 (which is 1)
    // array is now {16, 14, 18, 5}
    EXPECT_EQ(t.size(), 4);
    EXPECT_EQ(t.all_prod().val.val(), 53);

    t.insert(1, {mint(100), 1});
    // array is now {16, 100, 14, 18, 5}
    EXPECT_EQ(t.get(1).val.val(), 100);
    EXPECT_EQ(t.get(2).val.val(), 14);

    t.set(1, {mint(50), 1});
    EXPECT_EQ(t.get(1).val.val(), 50);

    auto vec = t.to_vec();
    std::vector<int> expected_vals = {16, 50, 14, 18, 5};
    ASSERT_EQ(vec.size(), expected_vals.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(vec[i].val.val(), expected_vals[i]);
    }
}
