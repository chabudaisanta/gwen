// clang-format off
#include "gwen/bbst/implicit_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen;

TEST(ImplicitTreapTest, BasicOperations) {
    ImplicitTreap<int> t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);

    t.push_back(10);
    t.push_back(20);
    t.push_front(5);
    // [5, 10, 20]
    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.get(0), 5);
    EXPECT_EQ(t.get(1), 10);
    EXPECT_EQ(t.get(2), 20);

    t.insert(1, 7);
    // [5, 7, 10, 20]
    EXPECT_EQ(t.get(1), 7);
    EXPECT_EQ(t.get(2), 10);

    t.erase(2);
    // [5, 7, 20]
    EXPECT_EQ(t.size(), 3);
    EXPECT_EQ(t.get(2), 20);

    t.set(1, 8);
    // [5, 8, 20]
    EXPECT_EQ(t.get(1), 8);

    auto vec = t.to_vec();
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 5);
    EXPECT_EQ(vec[1], 8);
    EXPECT_EQ(vec[2], 20);
}

TEST(ImplicitTreapTest, Reverse) {
    ImplicitTreap<int> t;
    for (int i = 0; i < 5; ++i) t.push_back(i); // [0, 1, 2, 3, 4]

    t.reverse(1, 4); // Reverse [1, 4) -> 1, 2, 3 becomes 3, 2, 1
    // [0, 3, 2, 1, 4]
    
    auto vec = t.to_vec();
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[1], 3);
    EXPECT_EQ(vec[2], 2);
    EXPECT_EQ(vec[3], 1);
    EXPECT_EQ(vec[4], 4);
}

TEST(ImplicitTreapTest, Concat) {
    ImplicitTreap<int> t1;
    t1.push_back(1);
    t1.push_back(2);

    ImplicitTreap<int> t2;
    t2.push_back(3);
    t2.push_back(4);

    t1.concat(t2);
    EXPECT_EQ(t1.size(), 4);
    EXPECT_EQ(t2.size(), 0);
    
    auto vec = t1.to_vec();
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[3], 4);
}
