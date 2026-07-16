// clang-format off
#include "gwen/bbst/sorted_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen;

TEST(SortedTreapTest, BasicOperations) {
    SortedTreap<int> t;
    EXPECT_TRUE(t.empty());

    t.insert(10);
    t.insert(5);
    t.insert(20);
    t.insert(10);  // multiset, allows duplicate

    EXPECT_EQ(t.size(), 4);
    EXPECT_TRUE(t.contains(5));
    EXPECT_TRUE(t.contains(10));
    EXPECT_TRUE(t.contains(20));
    EXPECT_FALSE(t.contains(15));

    EXPECT_EQ(t.kth(0), 5);
    EXPECT_EQ(t.kth(1), 10);
    EXPECT_EQ(t.kth(2), 10);
    EXPECT_EQ(t.kth(3), 20);

    EXPECT_EQ(t.count(10), 2);
    EXPECT_EQ(t.count(5), 1);
    EXPECT_EQ(t.count_lower(10), 1);  // only 5
    EXPECT_EQ(t.count_lower(11), 3);  // 5, 10, 10

    t.erase(10);
    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.count(10), 1);

    t.erase(10);
    EXPECT_EQ(t.size(), 2);
    EXPECT_EQ(t.count(10), 0);

    t.erase(10);  // erasing non-existent should not do anything
    EXPECT_EQ(t.size(), 2);

    auto vec = t.to_vec();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 5);
    EXPECT_EQ(vec[1], 20);
}
