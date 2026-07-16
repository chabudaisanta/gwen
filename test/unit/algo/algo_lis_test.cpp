#include <gtest/gtest.h>

#include "gwen/algo/lis.hpp"

using namespace gwen;

TEST(LisTest, Strict) {
    std::vector<i32> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    auto ans = lis_strict(vec);
    // possible LIS_strict: 1, 2, 3, 5
    // Actually: 1, 4, 5, 9 (length 4) or 1, 2, 6 (length 3)?
    // 1(idx 1), 4(idx 2), 5(idx 4), 9(idx 5) -> len 4
    // 1(idx 3), 2(idx 6), 6(idx 7)
    // 1, 2, 3, 5: 1(idx 1), 2(idx 6), 3(idx 9), 5(idx 10) -> length 4
    // Let's just check length and property
    EXPECT_EQ(ans.size(), 4);
    for (usize i = 0; i + 1 < ans.size(); ++i) {
        EXPECT_LT(ans[i], ans[i + 1]);
    }
}

TEST(LisTest, NonStrict) {
    std::vector<i32> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    auto ans = lis(vec);
    // length can be longer.
    // 1, 1, 5, 5? Wait: 1, 1, 2, 5, 5
    // 1(idx 1), 1(idx 3), 2(idx 6), 3(idx 9), 5(idx 10) -> length 5
    // Let's check length and property
    EXPECT_EQ(ans.size(), 5);
    for (usize i = 0; i + 1 < ans.size(); ++i) {
        EXPECT_LE(ans[i], ans[i + 1]);
    }
}

TEST(LisTest, Empty) {
    std::vector<i32> vec;
    EXPECT_TRUE(lis_strict(vec).empty());
    EXPECT_TRUE(lis(vec).empty());
}
