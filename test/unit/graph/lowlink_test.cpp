#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "gwen/graph/lowlink.hpp"

using namespace gwen;

namespace {

std::vector<std::vector<usize>> sorted_components(const Lowlink<Edge<i32>>& lowlink) {
    auto components = lowlink.vertex_biconnected_components();
    for (auto& component : components) std::sort(component.begin(), component.end());
    std::sort(components.begin(), components.end());
    return components;
}

std::vector<usize> sorted_bridges(const Lowlink<Edge<i32>>& lowlink) {
    auto bridges = lowlink.bridges();
    std::sort(bridges.begin(), bridges.end());
    return bridges;
}

}  // namespace

TEST(LowlinkTest, FindsArticulationPointsBridgesAndVertexBiconnectedComponents) {
    NonDirectedGraph<i32> g(7);
    g.add_edge(0, 1);  // 0
    g.add_edge(1, 2);  // 1
    g.add_edge(2, 0);  // 2
    g.add_edge(1, 3);  // 3
    g.add_edge(3, 4);  // 4
    g.add_edge(4, 5);  // 5
    g.add_edge(5, 3);  // 6
    g.add_edge(3, 6);  // 7
    g.build();

    const Lowlink lowlink(g);
    EXPECT_EQ(lowlink.articulation_points(), (std::vector<i32>{1, 3}));
    EXPECT_EQ(sorted_bridges(lowlink), (std::vector<usize>{3, 7}));
    EXPECT_EQ(sorted_components(lowlink),
              (std::vector<std::vector<usize>>{{0, 1, 2}, {3}, {4, 5, 6}, {7}}));
    EXPECT_EQ(lowlink.ord().size(), 7);
    EXPECT_EQ(lowlink.low().size(), 7);
}

TEST(LowlinkTest, HandlesParallelEdgesBySkippingOnlyTheParentEdge) {
    NonDirectedGraph<i32> g(3);
    g.add_edge(0, 1);  // 0
    g.add_edge(0, 1);  // 1
    g.add_edge(1, 2);  // 2
    g.build();

    const Lowlink lowlink(g);
    EXPECT_EQ(lowlink.articulation_points(), (std::vector<i32>{1}));
    EXPECT_EQ(sorted_bridges(lowlink), (std::vector<usize>{2}));
    EXPECT_EQ(sorted_components(lowlink), (std::vector<std::vector<usize>>{{0, 1}, {2}}));
}

TEST(LowlinkTest, HandlesSelfLoopsDisconnectedVerticesAndEmptyGraph) {
    NonDirectedGraph<i32> g(4);
    g.add_edge(0, 0);  // 0
    g.add_edge(0, 1);  // 1
    g.add_edge(2, 2);  // 2
    g.build();

    const Lowlink lowlink(g);
    EXPECT_TRUE(lowlink.articulation_points().empty());
    EXPECT_EQ(sorted_bridges(lowlink), (std::vector<usize>{1}));
    EXPECT_EQ(sorted_components(lowlink), (std::vector<std::vector<usize>>{{0}, {1}, {2}}));

    NonDirectedGraph<i32> empty(0);
    empty.build();
    const Lowlink empty_lowlink(empty);
    EXPECT_TRUE(empty_lowlink.ord().empty());
    EXPECT_TRUE(empty_lowlink.low().empty());
    EXPECT_TRUE(empty_lowlink.articulation_points().empty());
    EXPECT_TRUE(empty_lowlink.bridges().empty());
    EXPECT_TRUE(empty_lowlink.vertex_biconnected_components().empty());
}

TEST(LowlinkTest, RootIsArticulationOnlyWithAtLeastTwoDfsChildren) {
    NonDirectedGraph<i32> g(4);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(0, 3);
    g.build();

    const Lowlink lowlink(g);
    EXPECT_EQ(lowlink.articulation_points(), (std::vector<i32>{0}));
    EXPECT_EQ(sorted_bridges(lowlink), (std::vector<usize>{0, 1, 2}));
}
