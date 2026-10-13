// clang-format off
#include "gwen/graph/bellman_ford.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <limits>
#include <vector>

using namespace gwen;

TEST(BellmanFordTest, NegativeEdgesWithoutCycle) {
    DirectedGraph<i64> g(4);
    g.add_edge(0, 1, 4);
    g.add_edge(1, 2, -6);
    g.add_edge(0, 2, 5);
    g.build();

    const std::vector<i64> expected{0, 4, -2, std::numeric_limits<i64>::max()};
    EXPECT_EQ(bellman_ford(g, 0), expected);
    auto result = bellman_ford_with_negative_cycle(g, 0);
    EXPECT_EQ(result.dist, expected);
    EXPECT_EQ(result.negative_infinite, (std::vector<bool>{false, false, false, false}));
    EXPECT_EQ(vertices_reaching_negative_cycle(g), (std::vector<bool>{false, false, false, false}));
    EXPECT_EQ(vertices_reachable_from_negative_cycle(g), (std::vector<bool>{false, false, false, false}));
}

TEST(BellmanFordTest, CycleUpstreamAndDownstream) {
    DirectedGraph<i64> g(7);
    g.add_edge(0, 1, 2);
    g.add_edge(1, 2, 1);
    g.add_edge(2, 1, -3);
    g.add_edge(2, 3, 4);
    g.add_edge(4, 1, 7);
    g.add_edge(3, 6, 1);
    g.build();

    auto result = bellman_ford_with_negative_cycle(g, 0);
    EXPECT_EQ(result.negative_infinite, (std::vector<bool>{false, true, true, true, false, false, true}));
    EXPECT_EQ(result.dist[0], 0);
    EXPECT_EQ(result.dist[4], std::numeric_limits<i64>::max());
    EXPECT_EQ(result.dist[5], std::numeric_limits<i64>::max());
    EXPECT_EQ(vertices_reaching_negative_cycle(g), (std::vector<bool>{true, true, true, false, true, false, false}));
    EXPECT_EQ(vertices_reachable_from_negative_cycle(g),
              (std::vector<bool>{false, true, true, true, false, false, true}));
}

TEST(BellmanFordTest, UnreachableCycleDoesNotAffectSource) {
    DirectedGraph<i64> g(5);
    g.add_edge(0, 1, 3);
    g.add_edge(3, 4, -2);
    g.add_edge(4, 3, 1);
    g.build();

    const i64 inf = std::numeric_limits<i64>::max();
    EXPECT_EQ(bellman_ford(g, 0), (std::vector<i64>{0, 3, inf, inf, inf}));
    auto result = bellman_ford_with_negative_cycle(g, 0);
    EXPECT_EQ(result.dist, (std::vector<i64>{0, 3, inf, inf, inf}));
    EXPECT_EQ(result.negative_infinite, (std::vector<bool>{false, false, false, false, false}));
    EXPECT_EQ(vertices_reaching_negative_cycle(g), (std::vector<bool>{false, false, false, true, true}));
    EXPECT_EQ(vertices_reachable_from_negative_cycle(g), (std::vector<bool>{false, false, false, true, true}));
}

TEST(BellmanFordTest, MultipleCyclesAndSelfLoop) {
    DirectedGraph<i64> g(6);
    g.add_edge(0, 1, 0);
    g.add_edge(1, 1, -1);
    g.add_edge(1, 2, 0);
    g.add_edge(3, 4, -2);
    g.add_edge(4, 3, 1);
    g.add_edge(4, 5, 0);
    g.build();

    EXPECT_EQ(vertices_reaching_negative_cycle(g), (std::vector<bool>{true, true, false, true, true, false}));
    EXPECT_EQ(vertices_reachable_from_negative_cycle(g), (std::vector<bool>{false, true, true, true, true, true}));
    EXPECT_EQ(bellman_ford_with_negative_cycle(g, 0).negative_infinite,
              (std::vector<bool>{false, true, true, false, false, false}));
}

TEST(BellmanFordTest, UndirectedNegativeEdge) {
    NonDirectedGraph<i64> g(3);
    g.add_edge(0, 1, -1);
    g.build();

    EXPECT_EQ(vertices_reaching_negative_cycle(g), (std::vector<bool>{true, true, false}));
    EXPECT_EQ(vertices_reachable_from_negative_cycle(g), (std::vector<bool>{true, true, false}));
    EXPECT_EQ(bellman_ford_with_negative_cycle(g, 0).negative_infinite, (std::vector<bool>{true, true, false}));
}

TEST(BellmanFordTest, ExtremeNegativeWeightAndEmptyGraph) {
    DirectedGraph<i64> g(2);
    g.add_edge(0, 1, 0);
    g.add_edge(1, 1, -9000000000000000000LL);
    g.build();

    auto result = bellman_ford_with_negative_cycle(g, 0);
    EXPECT_EQ(result.negative_infinite, (std::vector<bool>{false, true}));
    EXPECT_EQ(result.dist, (std::vector<i64>{0, 0}));
    EXPECT_EQ(vertices_reaching_negative_cycle(g), (std::vector<bool>{true, true}));

    DirectedGraph<i64> empty(0);
    empty.build();
    EXPECT_TRUE(vertices_reaching_negative_cycle(empty).empty());
    EXPECT_TRUE(vertices_reachable_from_negative_cycle(empty).empty());
}
