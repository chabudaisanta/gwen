#include "gwen/automaton/weighted_automaton_factory.hpp"
#include <gtest/gtest.h>
#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"
#include <vector>

using namespace gwen;
using namespace gwen::automaton;

TEST(WeightedAutomatonFactoryTest, UsedDigitsCountExact) {
    using M = sum_monoid<i32>;
    auto a = used_digits_count_exact<10, M>(2);

    EXPECT_EQ(a.n, 1024);
    EXPECT_EQ(a.condition_count, 1);
    
    // Check initial state
    EXPECT_EQ(a.init.size(), 1);
    EXPECT_EQ(a.init[0].first, 0);

    // Check transitions and conditions
    EXPECT_EQ(a.edge(0, 3).first, 8); // 1 << 3
    EXPECT_EQ(a.edge(8, 5).first, 40); // (1 << 3) | (1 << 5)
    
    EXPECT_EQ(a.condition[0], 0ULL);
    EXPECT_EQ(a.condition[8], 0ULL);
    EXPECT_EQ(a.condition[40], 1ULL); // exactly 2 digits (3 and 5)
}

TEST(WeightedAutomatonFactoryTest, UsedDigitsCountLeq) {
    using M = sum_monoid<i32>;
    auto a = used_digits_count_leq<10, M>(2);

    EXPECT_EQ(a.n, 1024);
    EXPECT_EQ(a.condition_count, 1);

    EXPECT_EQ(a.condition[0], 1ULL);
    EXPECT_EQ(a.condition[8], 1ULL);
    EXPECT_EQ(a.condition[40], 1ULL); // exactly 2 digits (3 and 5)
    EXPECT_EQ(a.condition[41], 0ULL); // exactly 3 digits (0, 3, 5)
}

TEST(WeightedAutomatonFactoryTest, ContainsPattern) {
    using M = sum_monoid<i32>;
    std::vector<i32> pattern = {1, 2, 3};
    auto a = contains_pattern<10, M>(pattern);

    EXPECT_EQ(a.n, 4);
    EXPECT_EQ(a.condition_count, 1);
    
    // Not accepting until full pattern is matched
    EXPECT_EQ(a.condition[0], 0ULL);
    EXPECT_EQ(a.condition[1], 0ULL);
    EXPECT_EQ(a.condition[2], 0ULL);
    EXPECT_EQ(a.condition[3], 1ULL);

    // Initial state matching
    EXPECT_EQ(a.edge(0, 1).first, 1); // Match '1'
    EXPECT_EQ(a.edge(0, 2).first, 0); // Miss
    
    // Partial match and failure
    EXPECT_EQ(a.edge(1, 2).first, 2); // Match '1', '2'
    EXPECT_EQ(a.edge(1, 1).first, 1); // Miss '2' but matches prefix '1'
    EXPECT_EQ(a.edge(2, 3).first, 3); // Full match '1', '2', '3'
    EXPECT_EQ(a.edge(2, 1).first, 1); // Miss '3', matches prefix '1'
    
    // Absorbing state once matched
    EXPECT_EQ(a.edge(3, 0).first, 3);
    EXPECT_EQ(a.edge(3, 5).first, 3);
}

TEST(WeightedAutomatonFactoryTest, ContainsPatternOverlapping) {
    using M = sum_monoid<i32>;
    std::vector<i32> pattern = {1, 1, 1};
    auto a = contains_pattern<10, M>(pattern);

    EXPECT_EQ(a.edge(0, 1).first, 1);
    EXPECT_EQ(a.edge(1, 1).first, 2);
    EXPECT_EQ(a.edge(2, 1).first, 3);
    EXPECT_EQ(a.edge(2, 0).first, 0); // Mismatch after "11" goes to 0
}

TEST(WeightedAutomatonFactoryTest, DigitSumExact) {
    using M = sum_monoid<i32>;
    auto a = digit_sum_exact<10, M>(5);
    EXPECT_EQ(a.n, 6);
    EXPECT_EQ(a.condition[5], 1ULL);
    EXPECT_EQ(a.condition[4], 0ULL);
    EXPECT_EQ(a.edge(2, 3).first, 5);
}

TEST(WeightedAutomatonFactoryTest, DigitSumLeq) {
    using M = sum_monoid<i32>;
    auto a = digit_sum_leq<10, M>(5);
    EXPECT_EQ(a.n, 6);
    EXPECT_EQ(a.condition[5], 1ULL);
    EXPECT_EQ(a.condition[4], 1ULL);
}

TEST(WeightedAutomatonFactoryTest, DigitSumMod) {
    using M = sum_monoid<i32>;
    auto a = digit_sum_mod<10, M>(3, 1);
    EXPECT_EQ(a.n, 3);
    EXPECT_EQ(a.condition[1], 1ULL);
    EXPECT_EQ(a.condition[0], 0ULL);
    EXPECT_EQ(a.edge(2, 2).first, 1);
}

TEST(WeightedAutomatonFactoryTest, ExcludeDigits) {
    using M = sum_monoid<i32>;
    std::vector<i32> S = {4, 9};
    auto a = exclude_digits<10, M>(S);
    EXPECT_EQ(a.n, 1);
    EXPECT_EQ(a.condition[0], 1ULL);
    EXPECT_EQ(a.edge(0, 4).first, -1);
    EXPECT_EQ(a.edge(0, 9).first, -1);
    EXPECT_EQ(a.edge(0, 3).first, 0);
}

TEST(WeightedAutomatonFactoryTest, IncludeAllDigits) {
    using M = sum_monoid<i32>;
    std::vector<i32> S = {4, 9};
    auto a = include_all_digits<10, M>(S);
    EXPECT_EQ(a.n, 4);
    EXPECT_EQ(a.condition[3], 1ULL);
    EXPECT_EQ(a.condition[2], 0ULL);
    EXPECT_EQ(a.edge(0, 4).first, 1);
    EXPECT_EQ(a.edge(0, 9).first, 2);
}

TEST(WeightedAutomatonFactoryTest, ValueMod) {
    using M = sum_monoid<i32>;
    auto a = value_mod<10, M>(7, 3);
    EXPECT_EQ(a.n, 7);
    EXPECT_EQ(a.condition[3], 1ULL);
    EXPECT_EQ(a.condition[0], 0ULL);
    EXPECT_EQ(a.edge(5, 2).first, 3); // (5 * 10 + 2) % 7 == 52 % 7 == 3
}

TEST(WeightedAutomatonFactoryTest, NonZeroCountExact) {
    using M = sum_monoid<i32>;
    auto a = non_zero_count_exact<10, M>(2);
    EXPECT_EQ(a.n, 3);
    EXPECT_EQ(a.condition[2], 1ULL);
    EXPECT_EQ(a.condition[1], 0ULL);
    EXPECT_EQ(a.edge(1, 0).first, 1);
    EXPECT_EQ(a.edge(1, 5).first, 2);
}

TEST(WeightedAutomatonFactoryTest, NonZeroCountLeq) {
    using M = sum_monoid<i32>;
    auto a = non_zero_count_leq<10, M>(2);
    EXPECT_EQ(a.n, 3);
    EXPECT_EQ(a.condition[2], 1ULL);
    EXPECT_EQ(a.condition[1], 1ULL);
    EXPECT_EQ(a.condition[0], 1ULL);
}
