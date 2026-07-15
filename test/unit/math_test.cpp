// clang-format off
#include "gwen/math/integer.hpp"
#include "gwen/math/basic.hpp"
// clang-format on

#include "gwen/types.hpp"
#include <gtest/gtest.h>

using namespace gwen;

TEST(MathTest, IntegerDiv) {
    EXPECT_EQ(ceil_div(10, 3), 4);
    EXPECT_EQ(ceil_div(-10, 3), -3);
    EXPECT_EQ(ceil_div(10, -3), -3);
    EXPECT_EQ(ceil_div(-10, -3), 4);
    EXPECT_EQ(ceil_div(9, 3), 3);

    EXPECT_EQ(floor_div(10, 3), 3);
    EXPECT_EQ(floor_div(-10, 3), -4);
    EXPECT_EQ(floor_div(10, -3), -4);
    EXPECT_EQ(floor_div(-10, -3), 3);
    EXPECT_EQ(floor_div(9, 3), 3);

    EXPECT_EQ(out_div(10, 3), 4);
    EXPECT_EQ(out_div(-10, 3), -4);
    EXPECT_EQ(out_div(10, -3), -4);
    EXPECT_EQ(out_div(-10, -3), 4);
    EXPECT_EQ(out_div(9, 3), 3);
}

TEST(MathTest, BasicMath) {
    EXPECT_EQ(isqrt(0u), 0);
    EXPECT_EQ(isqrt(1u), 1);
    EXPECT_EQ(isqrt(2u), 1);
    EXPECT_EQ(isqrt(3u), 1);
    EXPECT_EQ(isqrt(4u), 2);
    EXPECT_EQ(isqrt(10u), 3);
    EXPECT_EQ(isqrt(16u), 4);
    EXPECT_EQ(isqrt(100u), 10);
    EXPECT_EQ(isqrt(99u), 9);

    EXPECT_EQ(sq(5), 25);
    EXPECT_EQ(sq(-5), 25);

    EXPECT_EQ(choose2(5), 10); // 5 * 4 / 2
    EXPECT_EQ(choose2(1), 0);
    EXPECT_EQ(choose2(2), 1);
}
