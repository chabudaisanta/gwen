// clang-format off
#include "gwen/automaton/automaton.hpp"
// clang-format on

#include <gtest/gtest.h>
#include "gwen/types.hpp"
#include "gwen/alge/monoid.hpp"

using namespace gwen;

// Tとしてi32を用いた場合にmonoidを満たすか確認
static_assert(monoid<automaton_monoid<i32, 2>>);

TEST(AutomatonTest, Identity) {
    using M = automaton_monoid<i32, 2>;
    auto e = M::e();

    EXPECT_EQ(e.n_states, 1);
    EXPECT_EQ(e.init, std::vector<i32>{0});
    EXPECT_EQ(e.accept, std::vector<i32>{0});
    EXPECT_EQ(e.edges.size(), 2);
    
    EXPECT_EQ(e.edges[0].first, 0);
    EXPECT_EQ(e.edges[0].second, 1);
    EXPECT_EQ(e.edges[1].first, 0);
    EXPECT_EQ(e.edges[1].second, 1);
}

TEST(AutomatonTest, ProductIdentity) {
    using M = automaton_monoid<i32, 2>;
    
    M::S a {
        .n_states = 2,
        .init = {0},
        .accept = {1},
        .edges = {
            {1, 2}, {0, 3}, // state 0: on 0 -> state 1 (cost 2), on 1 -> state 0 (cost 3)
            {1, 4}, {1, 5}  // state 1: on 0 -> state 1 (cost 4), on 1 -> state 1 (cost 5)
        }
    };
    
    auto e = M::e();
    
    // a * e
    auto prod_ae = M::op(a, e);
    EXPECT_EQ(prod_ae.n_states, 2);
    EXPECT_EQ(prod_ae.init, std::vector<i32>{0});
    EXPECT_EQ(prod_ae.accept, std::vector<i32>{1});
    EXPECT_EQ(prod_ae.edges.size(), 4);
    for (size_t i = 0; i < a.edges.size(); ++i) {
        EXPECT_EQ(prod_ae.edges[i].first, a.edges[i].first);
        EXPECT_EQ(prod_ae.edges[i].second, a.edges[i].second);
    }
    
    // e * a
    auto prod_ea = M::op(e, a);
    EXPECT_EQ(prod_ea.n_states, 2);
    EXPECT_EQ(prod_ea.init, std::vector<i32>{0});
    EXPECT_EQ(prod_ea.accept, std::vector<i32>{1});
    EXPECT_EQ(prod_ea.edges.size(), 4);
    for (size_t i = 0; i < a.edges.size(); ++i) {
        EXPECT_EQ(prod_ea.edges[i].first, a.edges[i].first);
        EXPECT_EQ(prod_ea.edges[i].second, a.edges[i].second);
    }
}

TEST(AutomatonTest, Product) {
    using M = automaton_monoid<i32, 2>;
    
    M::S a {
        .n_states = 2,
        .init = {0},
        .accept = {1},
        .edges = {
            {1, 2}, {0, 3},
            {1, 4}, {1, 5}
        }
    };

    M::S b {
        .n_states = 2,
        .init = {1},
        .accept = {0},
        .edges = {
            {0, 6}, {1, 7},
            {1, 8}, {0, 9}
        }
    };
    
    auto ab = M::op(a, b);
    
    EXPECT_EQ(ab.n_states, 4);
    EXPECT_EQ(ab.init, std::vector<i32>{1}); // {0} x {1} = {0*2 + 1} = {1}
    EXPECT_EQ(ab.accept, std::vector<i32>{2}); // {1} x {0} = {1*2 + 0} = {2}
    
    // Check transitions
    // State 0 (0,0):
    //   on 0: a->1(cost 2), b->0(cost 6) => next 1*2+0=2, cost 2*6=12
    EXPECT_EQ(ab.edges[0 * 2 + 0].first, 2);
    EXPECT_EQ(ab.edges[0 * 2 + 0].second, 12);
    //   on 1: a->0(cost 3), b->1(cost 7) => next 0*2+1=1, cost 3*7=21
    EXPECT_EQ(ab.edges[0 * 2 + 1].first, 1);
    EXPECT_EQ(ab.edges[0 * 2 + 1].second, 21);
    
    // State 1 (0,1):
    //   on 0: a->1(cost 2), b->1(cost 8) => next 1*2+1=3, cost 2*8=16
    EXPECT_EQ(ab.edges[1 * 2 + 0].first, 3);
    EXPECT_EQ(ab.edges[1 * 2 + 0].second, 16);
    //   on 1: a->0(cost 3), b->0(cost 9) => next 0*2+0=0, cost 3*9=27
    EXPECT_EQ(ab.edges[1 * 2 + 1].first, 0);
    EXPECT_EQ(ab.edges[1 * 2 + 1].second, 27);
}
