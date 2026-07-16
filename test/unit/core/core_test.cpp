// clang-format off
#include "gwen/core/macro.hpp"
#include "gwen/core/constants.hpp"
#include "gwen/core/grid.hpp"
#include "gwen/core/utils.hpp"
#include "gwen/core/bit.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <cmath>
#include <set>
#include <utility>
#include <vector>

#include "gwen/types.hpp"

using namespace gwen;

TEST(CoreTest, Utils) {
    int a = 10;
    EXPECT_TRUE(chmax(a, 20));
    EXPECT_EQ(a, 20);
    EXPECT_FALSE(chmax(a, 15));
    EXPECT_EQ(a, 20);

    EXPECT_TRUE(chmin(a, 5));
    EXPECT_EQ(a, 5);
    EXPECT_FALSE(chmin(a, 10));
    EXPECT_EQ(a, 5);

    EXPECT_TRUE(isIn(5, 0, 10));
    EXPECT_FALSE(isIn(10, 0, 10));
    EXPECT_FALSE(isIn(-1, 0, 10));

    EXPECT_TRUE(isOut(10, 0, 10));
    EXPECT_FALSE(isOut(5, 0, 10));

    EXPECT_TRUE(isOverlap(0, 5, 3, 8));
    EXPECT_FALSE(isOverlap(0, 5, 5, 10));
}

TEST(CoreTest, Grid) {
    std::vector<std::pair<i32, i32>> HW = {{3, 1}, {4, 1}, {5, 9}, {2, 6}, {5, 3}};
    for (auto [h, w] : HW) {
        std::set<std::pair<i32, i32>> se;
        for (i32 i = 0; i < 9; ++i) {
            auto [nh, nw] = mv_to(h, w, i);
            ASSERT_LE(std::abs(nh - h) + std::abs(nw - w), 2);
            se.emplace(nh, nw);
        }
        ASSERT_EQ(se.size(), 9);
    }
}

TEST(CoreTest, Bit) {
    int b = 0b1010;
    EXPECT_EQ(getBit(b, 1), 2);
    EXPECT_EQ(getBit(b, 0), 0);
    EXPECT_EQ(setBit(b, 0), 0b1011);
    EXPECT_EQ(clearBit(b, 1), 0b1000);
    EXPECT_EQ(toggleBit(b, 0), 0b1011);
}
