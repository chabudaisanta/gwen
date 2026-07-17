// clang-format off
#include "gwen/bbst/prod_sorted_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"

using namespace gwen;

TEST(ProdSortedTreapTest, BasicOperations) {
    ProdSortedTreap<int, sum_monoid<int>> t;
    EXPECT_TRUE(t.empty());

    t.insert(10, 100);
    t.insert(5, 50);
    t.insert(20, 200);

    EXPECT_EQ(t.size(), 3);
    EXPECT_TRUE(t.contains(5));
    EXPECT_TRUE(t.contains(10));
    EXPECT_TRUE(t.contains(20));

    // [5, 10, 20] -> [50, 100, 200]
    EXPECT_EQ(t.prod(5, 11), 150);
    EXPECT_EQ(t.prod(0, 100), 350);
    EXPECT_EQ(t.all_prod(), 350);

    t.insert(10, 100);  // multiple keys
    EXPECT_EQ(t.count(10), 2);
    EXPECT_EQ(t.prod(10, 11), 200);

    t.erase(10);  // erase one
    EXPECT_EQ(t.count(10), 1);
    EXPECT_EQ(t.prod(10, 11), 100);

    t.set(10, 99);  // erase all 10s and set 99
    EXPECT_EQ(t.count(10), 1);
    EXPECT_EQ(t.prod(10, 11), 99);

    t.erase_all(10);
    EXPECT_EQ(t.count(10), 0);
    EXPECT_EQ(t.all_prod(), 250);

    auto it_begin = t.begin();
    EXPECT_NE(it_begin, t.end());
    EXPECT_EQ(*it_begin, 5);

    auto lb20 = t.lower_bound(20);
    EXPECT_EQ(*lb20, 20);

    auto ub5 = t.upper_bound(5);
    EXPECT_EQ(*ub5, 20);

    auto it_end = t.end();
    --it_end;
    EXPECT_EQ(*it_end, 20);
}
