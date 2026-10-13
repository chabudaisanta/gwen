#include <gtest/gtest.h>

#include <limits>

#include "gwen/alge/acted_monoid.hpp"
#include "gwen/alge/range_add_range_max_monoid.hpp"
#include "gwen/alge/range_add_range_min_monoid.hpp"
#include "gwen/alge/range_add_range_sum_monoid.hpp"
#include "gwen/types.hpp"

using namespace gwen;

using SumMonoid = range_add_range_sum_monoid<i64>;
using MinMonoid = range_add_range_min_monoid<i64>;
using MaxMonoid = range_add_range_max_monoid<i64>;

static_assert(acted_monoid<SumMonoid>);
static_assert(acted_monoid<MinMonoid>);
static_assert(acted_monoid<MaxMonoid>);

TEST(RangeAddRangeSumMonoidTest, OperationsAndIdentity) {
    const auto e = SumMonoid::e();
    EXPECT_EQ(e.sum, 0);
    EXPECT_EQ(e.len, 0);

    const auto a = SumMonoid::unit(7);
    const auto b = SumMonoid::unit(3);
    const auto ab = SumMonoid::op(a, b);
    EXPECT_EQ(ab.sum, 10);
    EXPECT_EQ(ab.len, 2);

    const auto mapped_empty = SumMonoid::mapping(5, e);
    EXPECT_EQ(mapped_empty.sum, e.sum);
    EXPECT_EQ(mapped_empty.len, e.len);

    const auto mapped = SumMonoid::mapping(4, ab);
    EXPECT_EQ(mapped.sum, 18);
    EXPECT_EQ(mapped.len, 2);
    EXPECT_EQ(SumMonoid::composition(-2, 5), 3);
}

TEST(RangeAddRangeMinMonoidTest, SentinelAndActionComposition) {
    constexpr i64 e = std::numeric_limits<i64>::max();

    EXPECT_EQ(MinMonoid::e(), e);
    EXPECT_EQ(MinMonoid::mapping(-3, e), e);
    EXPECT_EQ(MinMonoid::op(e, 5), 5);
    EXPECT_EQ(MinMonoid::mapping(-2, e - 1), e - 3);
    EXPECT_EQ(MinMonoid::mapping(MinMonoid::composition(4, -1), 10), 13);
}

TEST(RangeAddRangeMaxMonoidTest, SentinelAndActionComposition) {
    constexpr i64 e = std::numeric_limits<i64>::lowest();

    EXPECT_EQ(MaxMonoid::e(), e);
    EXPECT_EQ(MaxMonoid::mapping(3, e), e);
    EXPECT_EQ(MaxMonoid::op(e, 5), 5);
    EXPECT_EQ(MaxMonoid::mapping(2, e + 1), e + 3);
    EXPECT_EQ(MaxMonoid::mapping(MaxMonoid::composition(-4, 1), 10), 7);
}
