#include "gwen/automaton/weighted_automaton.hpp"
#include <gtest/gtest.h>
#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(WeightedAutomatonTest, Basic) {
    using M = sum_monoid<i32>;
    WeightedAutomaton<2, M> a(2, 1);
    a.add_init(0, 0);
    a.set_condition(1, 1ULL);
    a.set_edge(0, 0, 1, 5);
    
    EXPECT_EQ(a.n, 2);
    EXPECT_EQ(a.condition_count, 1);
    EXPECT_EQ(a.init.size(), 1);
    EXPECT_EQ(a.init[0].first, 0);
    EXPECT_EQ(a.init[0].second, 0);
    EXPECT_EQ(a.condition[1], 1ULL);
    EXPECT_EQ(a.edge(0, 0).first, 1);
    EXPECT_EQ(a.edge(0, 0).second, 5);
}

TEST(WeightedAutomatonTest, ZeroCondition) {
    using M = sum_monoid<i32>;
    WeightedAutomaton<2, M> a(1, 0);
    a.add_init(0, 0);
    a.set_condition(0, 0ULL);
    
    EXPECT_EQ(a.n, 1);
    EXPECT_EQ(a.condition_count, 0);
}

TEST(WeightedAutomatonTest, ProductShift) {
    using M = sum_monoid<i32>;
    WeightedAutomaton<2, M> a(1, 2); // condition_count = 2
    a.add_init(0, 0);
    a.set_condition(0, 0b11ULL); // 2 bits
    a.set_edge(0, 0, 0, 10);
    
    WeightedAutomaton<2, M> b(1, 3); // condition_count = 3
    b.add_init(0, 0);
    b.set_condition(0, 0b101ULL); // 3 bits
    b.set_edge(0, 0, 0, 20);

    auto ab = a * b;

    EXPECT_EQ(ab.n, 1);
    EXPECT_EQ(ab.condition_count, 5);
    // b is shifted by a.condition_count(2) -> 0b10100
    // a is at the bottom -> 0b11
    // result -> 0b10111 = 23
    EXPECT_EQ(ab.condition[0], 23ULL);
    EXPECT_EQ(ab.edge(0, 0).second, 30); // 10 + 20
}

TEST(WeightedAutomatonTest, ProductAssociativity) {
    using M = sum_monoid<i32>;
    WeightedAutomaton<2, M> a(1, 2);
    a.add_init(0, 1);
    a.set_condition(0, 0b10ULL);
    a.set_edge(0, 0, 0, 1);
    
    WeightedAutomaton<2, M> b(1, 1);
    b.add_init(0, 2);
    b.set_condition(0, 0b1ULL);
    b.set_edge(0, 0, 0, 2);

    WeightedAutomaton<2, M> c(1, 3);
    c.add_init(0, 3);
    c.set_condition(0, 0b101ULL);
    c.set_edge(0, 0, 0, 3);

    auto ab_c = (a * b) * c;
    auto a_bc = a * (b * c);

    EXPECT_EQ(ab_c.n, a_bc.n);
    EXPECT_EQ(ab_c.condition_count, a_bc.condition_count);
    EXPECT_EQ(ab_c.condition[0], a_bc.condition[0]);
    EXPECT_EQ(ab_c.edge(0, 0).second, a_bc.edge(0, 0).second);
    EXPECT_EQ(ab_c.init[0].second, a_bc.init[0].second);
    
    // a: 2 bits (10), b: 1 bit (1), c: 3 bits (101)
    // combined: ccc b aa -> 101 1 10 -> 0b101110 = 46
    EXPECT_EQ(ab_c.condition[0], 46ULL);
    EXPECT_EQ(ab_c.condition_count, 6);
}

TEST(WeightedAutomatonTest, Trim) {
    using M = sum_monoid<i32>;
    WeightedAutomaton<2, M> a(3, 1);
    a.add_init(0, 0);
    a.set_condition(0, 0ULL);
    a.set_condition(1, 1ULL);
    a.set_condition(2, 0ULL); // unreachable
    
    a.set_edge(0, 0, 1, 5);
    // 2 is not reachable from 0

    a.trim();
    EXPECT_EQ(a.n, 2);
    EXPECT_EQ(a.init.size(), 1);
    EXPECT_EQ(a.init[0].first, 0);
    EXPECT_EQ(a.condition[1], 1ULL);
}

TEST(WeightedAutomatonTest, BuildWeightedAutomaton) {
    using M = sum_monoid<i32>;
    WeightedAutomaton<2, M> a(1, 1);
    a.add_init(0, 0);
    a.set_condition(0, 0b1ULL);
    WeightedAutomaton<2, M> b(1, 2);
    b.add_init(0, 0);
    b.set_condition(0, 0b10ULL);
    WeightedAutomaton<2, M> c(1, 1);
    c.add_init(0, 0);
    c.set_condition(0, 0b0ULL);

    std::vector<WeightedAutomaton<2, M>> automata = {a, b, c};
    auto res = build_weighted_automaton(automata);

    EXPECT_EQ(res.condition_count, 4);
    // c: 0, b: 10, a: 1 -> 0 10 1 -> 0b0101 = 5
    EXPECT_EQ(res.condition[0], 5ULL);
}
