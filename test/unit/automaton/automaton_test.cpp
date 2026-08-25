// clang-format off
#include "gwen/automaton/automaton.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

using namespace gwen;

// Tとしてi32を用いた場合にmonoidを満たすか確認 (今はコストを持たないのでSはAutomaton<base>)
static_assert(monoid<automaton_monoid<2>>);

TEST(AutomatonTest, Identity) {
    using M = automaton_monoid<2>;
    auto e = M::e();

    EXPECT_EQ(e.n, 1);
    EXPECT_EQ(e.init, std::vector<i32>{0});
    EXPECT_EQ(e.accept, std::vector<i32>{0});
    EXPECT_EQ(e.edges.size(), 2);

    EXPECT_EQ(e.edges[0], 0);
    EXPECT_EQ(e.edges[1], 0);
}

TEST(AutomatonTest, ProductIdentity) {
    using M = automaton_monoid<2>;

    M::S a(2);
    a.init = {0};
    a.accept = {1};
    a.edges = {
        1,  // state 0: on 0 -> state 1
        0,  // state 0: on 1 -> state 0
        1,  // state 1: on 0 -> state 1
        1   // state 1: on 1 -> state 1
    };

    auto e = M::e();

    // a * e
    auto prod_ae = M::op(a, e);
    EXPECT_EQ(prod_ae.n, 2);
    EXPECT_EQ(prod_ae.init, std::vector<i32>{0});
    EXPECT_EQ(prod_ae.accept, std::vector<i32>{1});
    EXPECT_EQ(prod_ae.edges.size(), 4);
    for (size_t i = 0; i < a.edges.size(); ++i) {
        EXPECT_EQ(prod_ae.edges[i], a.edges[i]);
    }

    // e * a
    auto prod_ea = M::op(e, a);
    EXPECT_EQ(prod_ea.n, 2);
    EXPECT_EQ(prod_ea.init, std::vector<i32>{0});
    EXPECT_EQ(prod_ea.accept, std::vector<i32>{1});
    EXPECT_EQ(prod_ea.edges.size(), 4);
    for (size_t i = 0; i < a.edges.size(); ++i) {
        EXPECT_EQ(prod_ea.edges[i], a.edges[i]);
    }
}

TEST(AutomatonTest, Product) {
    using M = automaton_monoid<2>;

    M::S a(2);
    a.init = {0};
    a.accept = {1};
    a.edges = {1, 0, 1, 1};

    M::S b(2);
    b.init = {1};
    b.accept = {0};
    b.edges = {0, 1, 1, 0};

    auto ab = M::op(a, b);

    EXPECT_EQ(ab.n, 4);
    EXPECT_EQ(ab.init, std::vector<i32>{1});    // {0} x {1} = {0*2 + 1} = {1}
    EXPECT_EQ(ab.accept, std::vector<i32>{2});  // {1} x {0} = {1*2 + 0} = {2}

    // Check transitions
    // State 0 (0,0):
    //   on 0: a->1, b->0 => next 1*2+0=2
    EXPECT_EQ(ab.edges[0 * 2 + 0], 2);
    //   on 1: a->0, b->1 => next 0*2+1=1
    EXPECT_EQ(ab.edges[0 * 2 + 1], 1);

    // State 1 (0,1):
    //   on 0: a->1, b->1 => next 1*2+1=3
    EXPECT_EQ(ab.edges[1 * 2 + 0], 3);
    //   on 1: a->0, b->0 => next 0*2+0=0
    EXPECT_EQ(ab.edges[1 * 2 + 1], 0);
}

TEST(AutomatonTest, Trim) {
    using M = automaton_monoid<2>;

    M::S a(5);
    // 0 -> 1 -> 2 -> 3
    // 4 is unreachable from init
    a.init = {0};
    a.accept = {3, 4};
    a.set_edge(0, 0, 1);
    a.set_edge(0, 1, 0);  // loop
    a.set_edge(1, 0, 2);
    a.set_edge(2, 1, 3);
    a.set_edge(4, 0, 3);  // transition from unreachable state

    a.trim();

    EXPECT_EQ(a.n, 4);
    EXPECT_EQ(a.init, std::vector<i32>{0});

    // original accept states were 3 and 4. 4 is removed.
    // 0: original 0
    // 1: original 1
    // 2: original 2
    // 3: original 3
    // so new accept should be {3}
    EXPECT_EQ(a.accept, std::vector<i32>{3});

    EXPECT_EQ(a.edge(0, 0), 1);
    EXPECT_EQ(a.edge(0, 1), 0);
    EXPECT_EQ(a.edge(1, 0), 2);
    EXPECT_EQ(a.edge(1, 1), -1);
    EXPECT_EQ(a.edge(2, 0), -1);
    EXPECT_EQ(a.edge(2, 1), 3);
    EXPECT_EQ(a.edge(3, 0), -1);
    EXPECT_EQ(a.edge(3, 1), -1);
}

TEST(AutomatonTest, ValidAllowsDuplicateInitialAndAcceptStates) {
    Automaton<2> a(1);
    a.init = {0, 0};
    a.accept = {0, 0};
    a.set_edge(0, 0, 0);
    a.set_edge(0, 1, 0);

    EXPECT_TRUE(a.valid());
}

TEST(AutomatonTest, ValidRejectsBrokenRepresentation) {
    Automaton<2> a(1);
    a.edges.pop_back();

    EXPECT_FALSE(a.valid());
}

TEST(AutomatonTest, ProductDoesNotTrimUnreachableStates) {
    Automaton<2> a(2);
    a.init = {0};
    a.accept = {0};
    a.set_edge(0, 0, 0);
    a.set_edge(0, 1, 0);

    auto product = a * a;

    EXPECT_EQ(product.n, 4);
    EXPECT_TRUE(product.valid());
}
