// clang-format off
#include "gwen/graph/graph.hpp"
// clang-format on

#include <gtest/gtest.h>
#include <vector>

using namespace gwen;

TEST(GraphTest, DirectedGraph) {
    DirectedGraph<i32> g(4);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 20);
    g.add_edge(1, 3, 30);
    g.add_edge(2, 3, 40);

    g.build();

    EXPECT_EQ(g.size(), 4);

    auto e0 = g.edges(0);
    ASSERT_EQ(e0.size(), 2);
    EXPECT_EQ(e0[0].v, 1);
    EXPECT_EQ(e0[0].w, 10);
    EXPECT_EQ(e0[1].v, 2);
    EXPECT_EQ(e0[1].w, 20);

    auto a0 = g.adjacent(0);
    std::vector<i32> v0(a0.begin(), a0.end());
    EXPECT_EQ(v0, (std::vector<i32>{1, 2}));

    auto idx0 = g[0];
    std::vector<i32> vi0(idx0.begin(), idx0.end());
    EXPECT_EQ(vi0, (std::vector<i32>{1, 2}));
}

TEST(GraphTest, NonDirectedGraph) {
    NonDirectedGraph<i32> g(3);
    g.add_edge(0, 1, 5);
    g.add_edge(1, 2, 15);
    g.build();

    EXPECT_EQ(g.size(), 3);

    auto e1 = g.edges(1);
    ASSERT_EQ(e1.size(), 2);
    
    std::vector<i32> v1(g[1].begin(), g[1].end());
    EXPECT_EQ(v1, (std::vector<i32>{0, 2}));
}

TEST(GraphTest, ConceptCheck) {
    static_assert(edge<Edge<i32>>);
    static_assert(graph<DirectedGraph<i32>>);
    static_assert(graph<NonDirectedGraph<i32>>);
}
