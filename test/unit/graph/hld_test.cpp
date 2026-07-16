// clang-format off
#include "gwen/graph/hld.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/graph/graph.hpp"

using namespace gwen;

TEST(HldTest, BasicOperations) {
    // Tree:
    //      0
    //     / \
    //    1   2
    //   / \   \
    //  3   4   5
    //           \
    //            6
    NonDirectedGraph<i32> g(7);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 5);
    g.add_edge(5, 6);
    g.build();

    Hld<NonDirectedGraph<i32>> hld(g, 0);

    // LCA
    EXPECT_EQ(hld.get_lca(3, 4), 1);
    EXPECT_EQ(hld.get_lca(3, 6), 0);
    EXPECT_EQ(hld.get_lca(5, 6), 5);
    EXPECT_EQ(hld.get_lca(0, 4), 0);

    // Dist
    EXPECT_EQ(hld.get_dist(3, 4), 2);  // 3-1-4
    EXPECT_EQ(hld.get_dist(3, 6), 5);  // 3-1-0-2-5-6
    EXPECT_EQ(hld.get_dist(5, 6), 1);
    EXPECT_EQ(hld.get_dist(0, 4), 2);  // 0-1-4

    // Kth Ancestor
    EXPECT_EQ(hld.kth_ancestor(6, 1), 5);
    EXPECT_EQ(hld.kth_ancestor(6, 2), 2);
    EXPECT_EQ(hld.kth_ancestor(6, 3), 0);
    EXPECT_EQ(hld.kth_ancestor(6, 4), -1);

    // Kth on Path
    EXPECT_EQ(hld.kth_on_path(3, 6, 0), 3);
    EXPECT_EQ(hld.kth_on_path(3, 6, 1), 1);
    EXPECT_EQ(hld.kth_on_path(3, 6, 2), 0);
    EXPECT_EQ(hld.kth_on_path(3, 6, 3), 2);
    EXPECT_EQ(hld.kth_on_path(3, 6, 4), 5);
    EXPECT_EQ(hld.kth_on_path(3, 6, 5), 6);
    EXPECT_EQ(hld.kth_on_path(3, 6, 6), -1);

    // Subtree
    auto st2 = hld.subtree(2);
    // The subtree size of 2 should be 3 (2, 5, 6)
    EXPECT_EQ(st2.second - st2.first, 3);

    // Verify path coverage
    auto p = hld.path(3, 6);
    i32 covered = 0;
    for (auto [l, r] : p) {
        covered += r - l;
    }
    // Path includes: 3, 1, 0, 2, 5, 6 -> 6 vertices
    EXPECT_EQ(covered, 6);
}
