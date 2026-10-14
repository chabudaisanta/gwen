#include <gtest/gtest.h>

#include <limits>
#include <vector>

#include "gwen/query/range_max.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(RangeMaxTest, PointUpdateAndRangeQuery) {
    RangeMax<i64> seg(std::vector<i64>{8, 3, 6, 5});
    EXPECT_EQ(seg.prod(0, 4), 8);
    EXPECT_EQ(seg.prod(1, 1), std::numeric_limits<i64>::lowest());
    EXPECT_EQ(seg.get(2), 6);

    seg.set(0, -2);
    seg.set(1, 9);
    EXPECT_EQ(seg.prod(0, 3), 9);
    EXPECT_EQ(seg.prod(0, 1), -2);
    EXPECT_EQ(seg.all_prod(), 9);
}

TEST(RangeMaxTest, IdentityAndFloatingPoint) {
    RangeMax<i64> initialized(3);
    EXPECT_EQ(initialized.all_prod(), std::numeric_limits<i64>::lowest());
    EXPECT_EQ(initialized.get(0), std::numeric_limits<i64>::lowest());

    RangeMax<i64> empty;
    EXPECT_EQ(empty.all_prod(), std::numeric_limits<i64>::lowest());
    EXPECT_EQ(empty.prod(0, 0), std::numeric_limits<i64>::lowest());

    RangeMax<double> floating(std::vector<double>{-3.5, 2.0, 1.25});
    EXPECT_EQ(floating.prod(0, 3), 2.0);
    EXPECT_EQ(floating.prod(1, 1), -std::numeric_limits<double>::infinity());
}
