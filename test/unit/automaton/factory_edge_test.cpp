#include <gtest/gtest.h>

#include <vector>

#include "gwen/automaton/factory.hpp"
#include "gwen/types.hpp"

using namespace gwen;
using namespace gwen::automaton;

TEST(FactoryEdgeTest, OutOfRangeDigitsKeepExistingBehavior) {
    const std::vector<i32> digits = {-1, 1, 10};
    const auto excluded = exclude_digits<10>(digits);
    const auto included = include_all_digits<10>(digits);

    EXPECT_EQ(excluded.edge(0, 1), -1);
    EXPECT_EQ(excluded.edge(0, 0), 0);
    EXPECT_EQ(included.n, 2);
    EXPECT_EQ(included.edge(0, 1), 1);
}

TEST(FactoryEdgeTest, OutOfRangeUsedDigitCountIsDefined) {
    const auto exact_negative = used_digits_count_exact<2>(-1);
    const auto exact_too_large = used_digits_count_exact<2>(3);
    const auto leq_negative = used_digits_count_leq<2>(-1);
    const auto leq_too_large = used_digits_count_leq<2>(3);

    EXPECT_TRUE(exact_negative.accept.empty());
    EXPECT_TRUE(exact_too_large.accept.empty());
    EXPECT_TRUE(leq_negative.accept.empty());
    EXPECT_EQ(leq_too_large.accept.size(), 5);
}

TEST(FactoryEdgeTest, LeadingZeroBehaviorIsExplicit) {
    const auto used = used_digits_count_exact<10>(0);
    const std::vector<i32> zero_one = {0, 1};
    const auto pattern = contains_pattern<10>(zero_one);

    EXPECT_EQ(used.edge(used.init.front(), 0), used.init.front());
    EXPECT_EQ(pattern.edge(0, 0), 1);
    EXPECT_EQ(pattern.edge(1, 1), 2);
}

#ifndef NDEBUG
TEST(FactoryEdgeDeathTest, RejectsInvalidArithmeticArguments) {
    EXPECT_DEATH((digit_sum_exact<10>(-1)), "");
    EXPECT_DEATH((digit_sum_mod<10>(0, 0)), "");
    EXPECT_DEATH((digit_sum_mod<10>(3, 3)), "");
    EXPECT_DEATH((value_mod<10>(0, 0)), "");
    EXPECT_DEATH((non_zero_count_exact<10>(-1)), "");
}

TEST(FactoryEdgeDeathTest, RejectsInvalidPatternDigit) {
    const std::vector<i32> pattern = {1, 10};
    EXPECT_DEATH((contains_pattern<10>(pattern)), "");
}
#endif
