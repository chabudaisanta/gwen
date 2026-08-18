#include "gwen/automaton/weighted_digit_dp.hpp"
#include <gtest/gtest.h>
#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(WeightedDigitDpTest, BasicCounting) {
    std::vector<i32> upper_bound = {1, 2}; // 12
    using M = sum_monoid<i32>;
    WeightedAutomaton<10, M> a(2, 1);
    a.add_init(0, 1);
    a.set_condition(0, 0ULL);
    a.set_condition(1, 1ULL);
    
    for (i32 c = 0; c < 10; ++c) {
        if (c == 1) a.set_edge(0, c, 1, 1);
        else a.set_edge(0, c, 0, 1);
        a.set_edge(1, c, 1, 1);
    }
    
    auto f = [](i32 dp_val, u64 condition) {
        return condition == 1ULL ? dp_val : 0;
    };
    
    i32 ans = run_weighted_digit_dp<i32>(upper_bound, a, f);
    EXPECT_EQ(ans, 4); // 1, 10, 11, 12
}

TEST(WeightedDigitDpTest, WeightMultiplication) {
    std::vector<i32> upper_bound = {9, 9}; // 99
    using M = sum_monoid<double>;
    WeightedAutomaton<10, M> a(2, 1);
    a.add_init(0, 1.0);
    a.set_condition(0, 1ULL); // All even
    a.set_condition(1, 0ULL); // Has odd
    
    for (i32 c = 0; c < 10; ++c) {
        double p = 0.1;
        if (c % 2 == 0) {
            a.set_edge(0, c, 0, p);
            a.set_edge(1, c, 1, p);
        } else {
            a.set_edge(0, c, 1, p);
            a.set_edge(1, c, 1, p);
        }
    }
    
    auto op_act = [](double dp_val, double w) {
        return dp_val * w;
    };
    
    auto f = [](double dp_val, u64 condition) {
        return condition == 1ULL ? dp_val : 0.0;
    };
    
    double ans = run_weighted_digit_dp<double>(upper_bound, a, f, op_act);
    EXPECT_NEAR(ans, 0.25, 1e-9);
}

TEST(WeightedDigitDpTest, ProductAutomaton) {
    std::vector<i32> upper_bound = {3, 0}; // 30
    
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
    
    auto ab = a * b; // condition: b is high(bit 1), a is low(bit 0) => 0b11
    
    auto f = [](i32 dp_val, u64 condition) {
        return condition == 0b11ULL ? dp_val : 0;
    };
    
    auto op_act = [](i32 dp_val, i32) {
        return dp_val; // 重みを無視してパスの数だけを足す
    };
    
    i32 ans = run_weighted_digit_dp<i32>(upper_bound, ab, f, op_act);
    EXPECT_EQ(ans, 2); // 3, 30
}
