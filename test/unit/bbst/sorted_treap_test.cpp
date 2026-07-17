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

    auto it_begin = t.begin();
    EXPECT_NE(it_begin, t.end());
    EXPECT_EQ(*it_begin, 5);
    auto it_next = it_begin;
    ++it_next;
    EXPECT_NE(it_next, t.end());
    EXPECT_EQ(*it_next, 20);
    ++it_next;
    EXPECT_EQ(it_next, t.end());

    auto it_end = t.end();
    --it_end;
    EXPECT_EQ(*it_end, 20);
    --it_end;
    EXPECT_EQ(*it_end, 5);

    auto lb20 = t.lower_bound(20);
    EXPECT_EQ(*lb20, 20);

    auto ub10 = t.upper_bound(10);
    EXPECT_EQ(*ub10, 20);

    auto ub20 = t.upper_bound(20);
    EXPECT_EQ(ub20, t.end());

    auto lb5 = t.lower_bound(5);
    EXPECT_EQ(*lb5, 5);

    // Test largest/smallest equivalent
    auto largest_20 = t.upper_bound(20);
    if (largest_20 != t.begin()) --largest_20;
    EXPECT_EQ(*largest_20, 20);

    auto largest_10 = t.upper_bound(10);
    if (largest_10 != t.begin()) --largest_10;
    EXPECT_EQ(*largest_10, 5);

    auto smallest_10 = t.lower_bound(10);
    EXPECT_EQ(*smallest_10, 20);
}
