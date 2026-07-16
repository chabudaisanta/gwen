// clang-format off
#include "gwen/graph/forest.hpp"
// clang-format on

#include <gtest/gtest.h>

namespace gwen {

TEST(GraphForestTest, BasicOperations) {
    DirectedGraph<int> G(6);
    // Forest:
    // Root 0: 0 -> 1, 0 -> 2, 2 -> 3
    // Root 4: 4 -> 5
    G.add_edge(0, 1);
    G.add_edge(0, 2);
    G.add_edge(2, 3);
    G.add_edge(4, 5);
    G.build();

    Forest<Edge<int>, true> forest(G);

    // Root check
    EXPECT_EQ(forest.get_root(0), 0);
    EXPECT_EQ(forest.get_root(1), 0);
    EXPECT_EQ(forest.get_root(3), 0);
    EXPECT_EQ(forest.get_root(4), 4);
    EXPECT_EQ(forest.get_root(5), 4);

    // Same check
    EXPECT_TRUE(forest.is_same(1, 3));
    EXPECT_FALSE(forest.is_same(1, 5));

    // Depth check
    EXPECT_EQ(forest.get_depth(0), 0);
    EXPECT_EQ(forest.get_depth(1), 1);
    EXPECT_EQ(forest.get_depth(2), 1);
    EXPECT_EQ(forest.get_depth(3), 2);
    EXPECT_EQ(forest.get_depth(4), 0);

    // Kth ancestor check
    EXPECT_EQ(forest.get_kth_ancestor(3, 1), 2);
    EXPECT_EQ(forest.get_kth_ancestor(3, 2), 0);
    EXPECT_EQ(forest.get_kth_ancestor(3, 3), -1);  // out of bounds

    // LCA check
    EXPECT_EQ(forest.get_lca(1, 2), 0);
    EXPECT_EQ(forest.get_lca(1, 3), 0);
    EXPECT_EQ(forest.get_lca(2, 3), 2);
    EXPECT_EQ(forest.get_lca(4, 5), 4);
    EXPECT_EQ(forest.get_lca(1, 5), -1);

    // Dist check
    EXPECT_EQ(forest.get_dist(1, 3), 3);  // 1 -> 0 -> 2 -> 3 (1+2=3 edges)
    EXPECT_EQ(forest.get_dist(0, 3), 2);
}

}  // namespace gwen
