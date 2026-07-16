// clang-format off
#include "gwen/graph/dijkstra.hpp"
// clang-format on

#include <gtest/gtest.h>
#include <vector>

using namespace gwen;

TEST(DijkstraTest, DirectedGraphShortestPath) {
    DirectedGraph<i32> g(5);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 3);
    g.add_edge(2, 1, 1);
    g.add_edge(1, 3, 2);
    g.add_edge(2, 3, 8);
    g.add_edge(2, 4, 2);
    g.add_edge(4, 3, 3);
    g.build();

    Dijkstra<DirectedGraph<i32>> dijk(g);
    auto dist = dijk.solve(0);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 4);  // 0 -> 2 -> 1
    EXPECT_EQ(dist[2], 3);  // 0 -> 2
    EXPECT_EQ(dist[3], 6);  // 0 -> 2 -> 1 -> 3
    EXPECT_EQ(dist[4], 5);  // 0 -> 2 -> 4

    auto spt = dijk.get_spt();
    EXPECT_EQ(spt[1].u, 2);
    EXPECT_EQ(spt[1].v, 1);
    EXPECT_EQ(spt[2].u, 0);
    EXPECT_EQ(spt[2].v, 2);
    EXPECT_EQ(spt[3].u, 1);
    EXPECT_EQ(spt[3].v, 3);
    EXPECT_EQ(spt[4].u, 2);
    EXPECT_EQ(spt[4].v, 4);
}

TEST(DijkstraTest, DisconnectedGraph) {
    DirectedGraph<i32> g(3);
    g.add_edge(0, 1, 5);
    g.build();

    Dijkstra<DirectedGraph<i32>> dijk(g);
    auto dist = dijk.solve(0);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 5);
    EXPECT_EQ(dist[2], Dijkstra<DirectedGraph<i32>>::INF);
}
