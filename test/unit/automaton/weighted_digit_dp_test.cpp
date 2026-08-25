#include "gwen/automaton/weighted_digit_dp.hpp"

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"
#include "gwen/automaton/factory.hpp"
#include "gwen/automaton/weighted_automaton_factory.hpp"
#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

using namespace gwen;

struct MinimalWeightedDigitDpValue {
    i32 val;

    MinimalWeightedDigitDpValue(i32 value) : val(value) {}
    MinimalWeightedDigitDpValue& operator+=(const MinimalWeightedDigitDpValue& rhs) {
        val += rhs.val;
        return *this;
    }
    bool operator==(const MinimalWeightedDigitDpValue&) const = default;
};

static_assert(weighted_digit_dp_value<MinimalWeightedDigitDpValue>);

struct MinimalDigitDpSumValue {
    i64 val;

    MinimalDigitDpSumValue(i64 value) : val(value) {}
    MinimalDigitDpSumValue operator+(const MinimalDigitDpSumValue& rhs) const { return val + rhs.val; }
    MinimalDigitDpSumValue operator*(const MinimalDigitDpSumValue& rhs) const { return val * rhs.val; }
    MinimalDigitDpSumValue& operator+=(const MinimalDigitDpSumValue& rhs) {
        val += rhs.val;
        return *this;
    }
    bool operator==(const MinimalDigitDpSumValue&) const = default;
};

static_assert(digit_dp_sum_value<MinimalDigitDpSumValue>);

struct MatrixSemiring2x2 {
    i64 a00;
    i64 a01;
    i64 a10;
    i64 a11;

    MatrixSemiring2x2(i64 scalar) : a00(scalar), a01(0), a10(0), a11(scalar) {}
    MatrixSemiring2x2(i64 a00, i64 a01, i64 a10, i64 a11) : a00(a00), a01(a01), a10(a10), a11(a11) {}

    MatrixSemiring2x2 operator+(const MatrixSemiring2x2& rhs) const {
        return {a00 + rhs.a00, a01 + rhs.a01, a10 + rhs.a10, a11 + rhs.a11};
    }
    MatrixSemiring2x2 operator*(const MatrixSemiring2x2& rhs) const {
        return {
            a00 * rhs.a00 + a01 * rhs.a10,
            a00 * rhs.a01 + a01 * rhs.a11,
            a10 * rhs.a00 + a11 * rhs.a10,
            a10 * rhs.a01 + a11 * rhs.a11,
        };
    }
    MatrixSemiring2x2& operator+=(const MatrixSemiring2x2& rhs) {
        a00 += rhs.a00;
        a01 += rhs.a01;
        a10 += rhs.a10;
        a11 += rhs.a11;
        return *this;
    }
    bool operator==(const MatrixSemiring2x2&) const = default;
};

static_assert(digit_dp_sum_value<MatrixSemiring2x2>);

TEST(WeightedDigitDpTest, BasicCounting) {
    std::vector<i32> upper_bound = {1, 2};  // 12
    using M = sum_monoid<i32>;
    WeightedAutomaton<10, M> a(2, 1);
    a.add_init(0, 1);
    a.set_condition(0, 0ULL);
    a.set_condition(1, 1ULL);

    for (i32 c = 0; c < 10; ++c) {
        if (c == 1)
            a.set_edge(0, c, 1, 1);
        else
            a.set_edge(0, c, 0, 1);
        a.set_edge(1, c, 1, 1);
    }

    auto f = [](i32 dp_val, u64 condition) { return condition == 1ULL ? dp_val : 0; };

    i32 ans = run_weighted_digit_dp<i32>(upper_bound, a, f);
    EXPECT_EQ(ans, 4);  // 1, 10, 11, 12
}

TEST(WeightedDigitDpTest, WeightMultiplication) {
    std::vector<i32> upper_bound = {9, 9};  // 99
    using M = sum_monoid<double>;
    WeightedAutomaton<10, M> a(2, 1);
    a.add_init(0, 1.0);
    a.set_condition(0, 1ULL);  // All even
    a.set_condition(1, 0ULL);  // Has odd

    for (i32 c = 0; c < 10; ++c) {
        double p = 0.1;
        if (c % 2 == 0) {
            a.set_edge(0, c, 0, p);
            a.set_edge(1, c, 1, p);
        }
        else {
            a.set_edge(0, c, 1, p);
            a.set_edge(1, c, 1, p);
        }
    }

    auto op_act = [](double dp_val, double w) { return dp_val * w; };

    auto f = [](double dp_val, u64 condition) { return condition == 1ULL ? dp_val : 0.0; };

    double ans = run_weighted_digit_dp<double>(upper_bound, a, f, op_act);
    EXPECT_NEAR(ans, 0.25, 1e-9);
}

TEST(WeightedDigitDpTest, ProductAutomaton) {
    std::vector<i32> upper_bound = {3, 0};  // 30

    using M = max_monoid<i32>;
    WeightedAutomaton<10, M> a(2, 1);
    a.add_init(0, 1);
    a.set_condition(0, 0ULL);
    a.set_condition(1, 1ULL);
    for (i32 c = 0; c < 10; ++c) {
        a.set_edge(0, c, (c == 3) ? 1 : 0, 1);
        a.set_edge(1, c, 1, 1);
    }

    WeightedAutomaton<10, M> b(3, 1);
    b.add_init(0, 1);
    for (i32 i = 0; i < 3; ++i) {
        b.set_condition(i, (i == 0) ? 1ULL : 0ULL);
        for (i32 c = 0; c < 10; ++c) {
            b.set_edge(i, c, (i + c) % 3, 1);
        }
    }

    auto ab = a * b;  // condition: b is high(bit 1), a is low(bit 0) => 0b11

    auto f = [](i32 dp_val, u64 condition) { return condition == 0b11ULL ? dp_val : 0; };

    auto op_act = [](i32 dp_val, i32) {
        return dp_val;  // 重みを無視してパスの数だけを足す
    };

    i32 ans = run_weighted_digit_dp<i32>(upper_bound, ab, f, op_act);
    EXPECT_EQ(ans, 2);  // 3, 30
}

TEST(WeightedDigitDpTest, ExplicitInitialValueWithFactory) {
    using M = mul_monoid<i32>;
    const auto a = automaton::digit_sum_exact<10, M>(1);
    const std::vector<i32> upper_bound = {1, 2};
    const auto aggregate = [](const i32& value, u64 condition) { return condition == 1ULL ? value : 0; };
    const auto act = [](const i32& value, const i32& weight) { return value * weight; };

    const i32 ans = run_weighted_digit_dp<i32>(upper_bound, a, 1, aggregate, act);

    EXPECT_EQ(ans, 2);  // 01, 10
}

TEST(WeightedDigitDpTest, ExistingOverloadKeepsBehavior) {
    using M = mul_monoid<i32>;
    const auto a = automaton::digit_sum_exact<10, M>(1);
    const std::vector<i32> upper_bound = {1, 2};
    const auto aggregate = [](const i32& value, u64 condition) { return condition == 1ULL ? value : 0; };

    const i32 ans = run_weighted_digit_dp<i32>(upper_bound, a, aggregate);

    EXPECT_EQ(ans, 2);  // 01, 10
}

TEST(WeightedDigitDpTest, RequiresOnlyWeightedDigitDpValueOperations) {
    using M = mul_monoid<i32>;
    const auto a = automaton::digit_sum_exact<10, M>(1);
    const std::vector<i32> upper_bound = {1, 2};
    const auto aggregate = [](const MinimalWeightedDigitDpValue& value, u64 condition) {
        return condition == 1ULL ? value : MinimalWeightedDigitDpValue(0);
    };
    const auto act = [](const MinimalWeightedDigitDpValue& value, const i32& weight) {
        return MinimalWeightedDigitDpValue(value.val * weight);
    };

    const auto ans = run_weighted_digit_dp<MinimalWeightedDigitDpValue>(upper_bound, a, MinimalWeightedDigitDpValue(1),
                                                                        aggregate, act);

    EXPECT_EQ(ans.val, 2);
}

TEST(DigitDpSumTest, SumsAllAcceptedValues) {
    Automaton<10> a(1);
    a.add_init(0);
    a.add_accept(0);
    for (i32 digit = 0; digit < 10; ++digit) a.set_edge(0, digit, 0);

    const i64 ans = run_digit_dp_sum<i64>({1, 2}, a);

    EXPECT_EQ(ans, 78);  // 0 + 1 + ... + 12
}

TEST(DigitDpSumTest, SumsValuesAcceptedByDigitSumCondition) {
    const auto a = automaton::digit_sum_mod<10>(3, 0);

    const i64 ans = run_digit_dp_sum<i64>({1, 2}, a);

    EXPECT_EQ(ans, 30);  // 0 + 3 + 6 + 9 + 12
}

TEST(DigitDpSumTest, SupportsNonDecimalBase) {
    const auto a = automaton::non_zero_count_exact<2>(2);

    const i64 ans = run_digit_dp_sum<i64>({1, 0, 1}, a);  // 5

    EXPECT_EQ(ans, 8);  // 3 + 5
}

TEST(DigitDpSumTest, ReturnsZeroForEmptyAcceptSet) {
    Automaton<10> a(1);
    a.add_init(0);
    for (i32 digit = 0; digit < 10; ++digit) a.set_edge(0, digit, 0);

    EXPECT_EQ(run_digit_dp_sum<i64>({9}, a), 0);
}

TEST(DigitDpSumTest, HandlesMissingTransitions) {
    Automaton<10> a(1);
    a.add_init(0);
    a.add_accept(0);
    a.set_edge(0, 0, 0);

    EXPECT_EQ(run_digit_dp_sum<i64>({9}, a), 0);  // 0 only
}

TEST(DigitDpSumTest, DuplicateStatesActAsMultiplicity) {
    Automaton<2> a(1);
    a.init = {0, 0};
    a.accept = {0, 0, 0};
    a.set_edge(0, 0, 0);
    a.set_edge(0, 1, 0);

    const i64 ans = run_digit_dp_sum<i64>({1}, a);

    EXPECT_EQ(ans, 6);  // value 1 * 2 initial states * 3 accepting states
}

TEST(DigitDpSumTest, AffineCompositionFollowsDigitOrder) {
    using M = automaton_detail::append_affine_monoid<i64>;
    using State = automaton_detail::DigitDpSumState<i64>;
    const M::S first{10, 1};
    const M::S second{10, 2};

    const M::S composed = M::op(first, second);
    EXPECT_EQ(composed.a, 100);
    EXPECT_EQ(composed.b, 12);

    const State initial{1, 0};
    const State sequential = automaton_detail::apply_digit_dp_sum_affine(
        automaton_detail::apply_digit_dp_sum_affine(initial, first), second);
    const State combined = automaton_detail::apply_digit_dp_sum_affine(initial, composed);
    EXPECT_EQ(sequential, combined);
    EXPECT_EQ(combined.count, 1);
    EXPECT_EQ(combined.sum, 12);
}

TEST(DigitDpSumTest, AffineActionSupportsNonCommutativeSemiring) {
    using T = MatrixSemiring2x2;
    using M = automaton_detail::append_affine_monoid<T>;
    using State = automaton_detail::DigitDpSumState<T>;
    const State initial{T{1, 2, 0, 1}, T{0, 1, 1, 0}};
    const M::S first{T{1, 1, 0, 1}, T{0, 1, 1, 1}};
    const M::S second{T{2, 0, 1, 1}, T{1, 0, 1, 2}};

    const State applied = automaton_detail::apply_digit_dp_sum_affine(initial, first);
    const T expected_sum = first.a * initial.sum + first.b * initial.count;
    const T reversed_sum = initial.sum * first.a + initial.count * first.b;
    EXPECT_EQ(applied.sum, expected_sum);
    EXPECT_FALSE(applied.sum == reversed_sum);

    const State sequential = automaton_detail::apply_digit_dp_sum_affine(
        automaton_detail::apply_digit_dp_sum_affine(initial, first), second);
    const State combined = automaton_detail::apply_digit_dp_sum_affine(initial, M::op(first, second));
    EXPECT_EQ(sequential, combined);
}

TEST(DigitDpSumTest, SupportsMinimalSemiringValue) {
    Automaton<10> a(1);
    a.add_init(0);
    a.add_accept(0);
    for (i32 digit = 0; digit < 10; ++digit) a.set_edge(0, digit, 0);

    const auto ans = run_digit_dp_sum<MinimalDigitDpSumValue>({1, 2}, a);

    EXPECT_EQ(ans.val, 78);
}

TEST(DigitDpSumTest, SupportsGwenModInt) {
    using mint = DynamicModInt64;
    mint::set_mod(1000000007);
    Automaton<10> a(1);
    a.add_init(0);
    a.add_accept(0);
    for (i32 digit = 0; digit < 10; ++digit) a.set_edge(0, digit, 0);

    const mint ans = run_digit_dp_sum<mint>({1, 2}, a);

    EXPECT_EQ(ans.val(), 78ULL);
}

#ifndef NDEBUG
TEST(WeightedDigitDpDeathTest, RejectsOutOfRangeDigit) {
    using M = mul_monoid<i32>;
    const auto a = automaton::digit_sum_exact<2, M>(1);
    const auto aggregate = [](const i32& value, u64) { return value; };

    EXPECT_DEATH((run_weighted_digit_dp<i32>({2}, a, aggregate)), "");
}

TEST(DigitDpSumDeathTest, RejectsOutOfRangeDigit) {
    Automaton<2> a(1);
    a.add_init(0);
    a.add_accept(0);
    a.set_edge(0, 0, 0);
    a.set_edge(0, 1, 0);

    EXPECT_DEATH((run_digit_dp_sum<i64>({2}, a)), "");
}
#endif
