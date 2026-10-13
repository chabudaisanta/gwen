#include <gtest/gtest.h>

#include <limits>
#include <vector>

#include "gwen/query/range_add_range_max.hpp"
#include "gwen/query/range_add_range_min.hpp"
#include "gwen/query/range_add_range_sum.hpp"
#include "gwen/query/range_min.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(RangeAddRangeSumTest, RangeAndPointOperations) {
    RangeAddRangeSum<i64> seg(std::vector<i64>{1, 2, 3, 4});
    EXPECT_EQ(seg.prod(1, 4), 9);
    EXPECT_EQ(seg.prod(2, 2), 0);

    seg.add(1, 4, 5);
    EXPECT_EQ(seg.get(0), 1);
    EXPECT_EQ(seg.get(2), 8);
    EXPECT_EQ(seg.all_prod(), 25);

    seg.add(0, -1);
    EXPECT_EQ(seg.prod(0, 2), 7);
}

TEST(RangeAddRangeSumTest, ZeroInitialized) {
    RangeAddRangeSum<i64> seg(3);
    EXPECT_EQ(seg.all_prod(), 0);
    seg.add(0, 3, 2);
    EXPECT_EQ(seg.prod(0, 3), 6);
}

TEST(RangeAddRangeMinTest, RangeAndEmptyQueries) {
    RangeAddRangeMin<i64> seg(std::vector<i64>{8, 3, 6, 5});
    EXPECT_EQ(seg.prod(0, 4), 3);
    EXPECT_EQ(seg.prod(2, 2), std::numeric_limits<i64>::max());

    seg.add(0, 4, 2);
    seg.add(1, 4, -4);
    EXPECT_EQ(seg.prod(0, 4), 1);
    EXPECT_EQ(seg.get(3), 3);
    EXPECT_EQ(seg.all_prod(), 1);
}

TEST(RangeAddRangeMinTest, ZeroInitialized) {
    RangeAddRangeMin<i64> seg(3);
    EXPECT_EQ(seg.all_prod(), 0);
    seg.add(1, 3, 4);
    EXPECT_EQ(seg.prod(0, 3), 0);
}

TEST(RangeAddRangeMaxTest, RangeAndEmptyQueries) {
    RangeAddRangeMax<i64> seg(std::vector<i64>{8, 3, 6, 5});
    EXPECT_EQ(seg.prod(0, 4), 8);
    EXPECT_EQ(seg.prod(2, 2), std::numeric_limits<i64>::lowest());

    seg.add(0, 4, -5);
    seg.add(1, 4, 3);
    EXPECT_EQ(seg.prod(0, 4), 4);
    EXPECT_EQ(seg.get(0), 3);
    EXPECT_EQ(seg.all_prod(), 4);
}

TEST(RangeMinTest, AliasSupportsPointSetAndRangeQuery) {
    RangeMin<i64> seg(std::vector<i64>{8, 3, 6, 5});
    EXPECT_EQ(seg.prod(0, 4), 3);
    seg.set(1, 9);
    EXPECT_EQ(seg.prod(0, 4), 5);
}
