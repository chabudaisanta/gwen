// clang-format off
#include "utils/naive_array.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen::test;

TEST(NaiveArrayTest, BasicOperations) {
    NaiveArray<int> arr(5, 0);  // [0, 0, 0, 0, 0]

    EXPECT_EQ(arr.size(), 5);

    arr.set(2, 10);
    EXPECT_EQ(arr.get(2), 10);

    arr.add(2, 5);
    EXPECT_EQ(arr.get(2), 15);

    arr.range_add(1, 4, 2);  // [0, 2, 17, 2, 0]

    EXPECT_EQ(arr.get(1), 2);
    EXPECT_EQ(arr.get(2), 17);
    EXPECT_EQ(arr.get(3), 2);
    EXPECT_EQ(arr.get(4), 0);

    EXPECT_EQ(arr.sum(0, 5), 21);
    EXPECT_EQ(arr.min(0, 5, 100), 0);
    EXPECT_EQ(arr.max(0, 5, -100), 17);

    arr.range_update(0, 2, 5);  // [5, 5, 17, 2, 0]
    EXPECT_EQ(arr.sum(0, 2), 10);
}

TEST(NaiveArrayTest, FoldOperation) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    NaiveArray<int> arr(v);

    auto op = [](int a, int b) { return a ^ b; };
    EXPECT_EQ(arr.fold(0, 5, op, 0), 1 ^ 2 ^ 3 ^ 4 ^ 5);
}
