// clang-format off
#include "gwen/graph/tree_lca.hpp"
// clang-format on

#include <gtest/gtest.h>

namespace gwen {

TEST(GraphTreeLcaTest, BasicOperations) {
    DirectedGraph<int> G(6);
    // Tree:
    // 0 -> 1, 0 -> 2, 2 -> 3, 2 -> 4, 4 -> 5
    G.add_edge(0, 1);
    G.add_edge(0, 2);
    G.add_edge(2, 3);
    G.add_edge(2, 4);
    G.add_edge(4, 5);
    G.build();

    TreeLca<Edge<int>, true> lca(0, G);

    // Depth check
    EXPECT_EQ(lca.depth(0), 0);
    EXPECT_EQ(lca.depth(1), 1);
    EXPECT_EQ(lca.depth(2), 1);
    EXPECT_EQ(lca.depth(3), 2);
    EXPECT_EQ(lca.depth(4), 2);
    EXPECT_EQ(lca.depth(5), 3);

    // LCA check
    EXPECT_EQ(lca.lca(1, 2), 0);
    EXPECT_EQ(lca.lca(3, 4), 2);
    EXPECT_EQ(lca.lca(3, 5), 2);
    EXPECT_EQ(lca.lca(1, 5), 0);

    // Kth ancestor check
    EXPECT_EQ(lca.kth_anc(5, 1), 4);
    EXPECT_EQ(lca.kth_anc(5, 2), 2);
    EXPECT_EQ(lca.kth_anc(5, 3), 0);
    EXPECT_EQ(lca.kth_anc(5, 4), -1);

    // Length check
    // len = depth(u) + depth(v) - 2 * depth(lca) + 1
    // depth(1) = 1, depth(5) = 3, lca = 0 (depth 0). len = 1 + 3 - 0 + 1 = 5.
    EXPECT_EQ(lca.len(1, 5), 5);

    EXPECT_EQ(lca.len(3, 4), 3);  // 3, 2, 4
}

}  // namespace gwen
