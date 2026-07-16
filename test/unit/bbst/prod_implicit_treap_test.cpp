// clang-format off
#include "gwen/bbst/prod_implicit_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"

using namespace gwen;

TEST(ProdImplicitTreapTest, SumMonoid) {
    ProdImplicitTreap<sum_monoid<int>> t;
    t.push_back(1);
    t.push_back(2);
    t.push_back(3);
    t.push_back(4);

    // [1, 2, 3, 4]
    EXPECT_EQ(t.prod(0, 4), 10);
    EXPECT_EQ(t.prod(1, 3), 5);  // 2 + 3

    t.reverse(1, 4);
    // [1, 4, 3, 2]
    EXPECT_EQ(t.prod(1, 3), 7);  // 4 + 3
    EXPECT_EQ(t.get(1), 4);
}
