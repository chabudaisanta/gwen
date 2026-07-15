// clang-format off
#include "gwen/core/macro.hpp"
#include "gwen/core/constants.hpp"
#include "gwen/core/grid.hpp"
#include "gwen/core/utils.hpp"
#include "gwen/core/bit.hpp"
// clang-format on

#include <gtest/gtest.h>

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
    auto [nh, nw] = mv_to(5, 5, 0);
    EXPECT_EQ(nh, 4);
    EXPECT_EQ(nw, 5);
}

TEST(CoreTest, Bit) {
    int b = 0b1010;
    EXPECT_EQ(getBit(b, 1), 2);
    EXPECT_EQ(getBit(b, 0), 0);
    EXPECT_EQ(setBit(b, 0), 0b1011);
    EXPECT_EQ(clearBit(b, 1), 0b1000);
    EXPECT_EQ(toggleBit(b, 0), 0b1011);
}
