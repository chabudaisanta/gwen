// clang-format off
#include "gwen/graph/tree_diameter.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <vector>

namespace gwen {

static_assert(requires(const NonDirectedGraph<i64>& g) { tree_diameter(g); });

TEST(TreeDiameterTest, SingleVertexAndZeroWeightTree) {
    NonDirectedGraph<i64> single(1);
    single.build();

    const auto one = tree_diameter(single);
    EXPECT_EQ(one.distance, 0);
    EXPECT_EQ(one.endpoint_u, 0);
    EXPECT_EQ(one.endpoint_v, 0);
    EXPECT_EQ(one.path, (std::vector<i32>{0}));
    EXPECT_EQ(one.centers, (std::vector<i32>{0}));

    NonDirectedGraph<i64> zero(4);
    zero.add_edge(0, 1, 0);
    zero.add_edge(1, 2, 0);
    zero.add_edge(1, 3, 0);
    zero.build();

    const auto result = tree_diameter(zero);
    EXPECT_EQ(result.distance, 0);
    EXPECT_EQ(result.path, (std::vector<i32>{0}));
    EXPECT_EQ(result.centers, (std::vector<i32>{0}));
}

TEST(TreeDiameterTest, FindsWeightedDiameterAndVertexCenter) {
    NonDirectedGraph<i64> g(5);
    g.add_edge(0, 1, 2);
    g.add_edge(1, 2, 2);
    g.add_edge(1, 3, 1);
    g.add_edge(3, 4, 0);
    g.build();

    const auto result = tree_diameter(g);
    EXPECT_EQ(result.distance, 4);
    EXPECT_EQ(result.path, (std::vector<i32>{2, 1, 0}));
    EXPECT_EQ(result.centers, (std::vector<i32>{1}));
}

TEST(TreeDiameterTest, ReturnsEndpointsOfEdgeContainingCenter) {
    NonDirectedGraph<i64> g(4);
    g.add_edge(0, 1, 2);
    g.add_edge(1, 2, 3);
    g.add_edge(2, 3, 4);
    g.build();

    const auto result = tree_diameter(g);
    EXPECT_EQ(result.distance, 9);
    const std::vector<i32> forward{0, 1, 2, 3};
    const std::vector<i32> backward{3, 2, 1, 0};
    EXPECT_TRUE(result.path == forward || result.path == backward);
    EXPECT_TRUE(result.centers == (std::vector<i32>{1, 2}) || result.centers == (std::vector<i32>{2, 1}));
}

TEST(TreeDiameterTest, HandlesZeroWeightSegmentsAroundCenter) {
    NonDirectedGraph<i64> g(6);
    g.add_edge(0, 1, 3);
    g.add_edge(1, 2, 0);
    g.add_edge(2, 3, 0);
    g.add_edge(3, 4, 3);
    g.add_edge(3, 5, 1);
    g.build();

    const auto result = tree_diameter(g);
    EXPECT_EQ(result.distance, 6);
    const std::vector<i32> forward{0, 1, 2, 3, 4};
    const std::vector<i32> backward{4, 3, 2, 1, 0};
    EXPECT_TRUE(result.path == forward || result.path == backward);
    ASSERT_EQ(result.centers.size(), 1);
    EXPECT_TRUE(result.centers[0] >= 1 && result.centers[0] <= 3);
}

TEST(TreeDiameterTest, UsesWeightedDistanceRatherThanHopCount) {
    NonDirectedGraph<i64> g(4);
    g.add_edge(0, 1, 100);
    g.add_edge(0, 2, 1);
    g.add_edge(2, 3, 1);
    g.build();

    const auto result = tree_diameter(g);
    EXPECT_EQ(result.distance, 102);
    EXPECT_EQ(result.path, (std::vector<i32>{1, 0, 2, 3}));
    EXPECT_EQ(result.centers, (std::vector<i32>{1, 0}));
}

}  // namespace gwen
