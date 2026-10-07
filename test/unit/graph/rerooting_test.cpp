// clang-format off
#include "gwen/graph/rerooting.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

using namespace gwen;

namespace {
struct string_concat_monoid {
    using S = std::string;
    static S op(S&& a, S&& b) { return a + b; }
    static S e() { return {}; }
};
}  // namespace

TEST(RerootingTest, EmptyAndSingleVertex) {
    const auto merge = [](i64 a, i64 b) { return std::max(a, b); };
    const auto add_vertex = [](i64 value, i32) { return value; };
    const auto lift = [](i64 value, const Edge<i64>& e) { return value + e.w; };

    NonDirectedGraph<i64> empty(0);
    empty.build();
    EXPECT_TRUE(rerooting(empty, i64{0}, merge, add_vertex, lift).empty());

    NonDirectedGraph<i64> single(1);
    single.build();
    EXPECT_EQ(rerooting(single, i64{0}, merge, add_vertex, lift), (std::vector<i64>{0}));
}

TEST(RerootingTest, WeightedHeight) {
    NonDirectedGraph<i64> g(4);
    g.add_edge(0, 1, 2);
    g.add_edge(1, 2, 1);
    g.add_edge(1, 3, 3);
    g.build();

    const auto answer = rerooting(
        g, i64{0}, [](i64 a, i64 b) { return std::max(a, b); }, [](i64 value, i32) { return value; },
        [](i64 value, const Edge<i64>& e) { return value + e.w; });
    EXPECT_EQ(answer, (std::vector<i64>{5, 3, 4, 5}));

    const auto monoid_answer = rerooting<max_monoid<i64>>(
        g, [](i64 value, i32) { return std::max(value, i64{0}); },
        [](i64 value, const Edge<i64>& e) { return value + e.w; });
    EXPECT_EQ(monoid_answer, answer);
}

TEST(RerootingTest, MonoidSingleVertex) {
    NonDirectedGraph<i64> g(1);
    g.build();
    const auto answer = rerooting<max_monoid<i64>>(
        g, [](i64 value, i32) { return std::max(value, i64{0}); },
        [](i64 value, const Edge<i64>& e) { return value + e.w; });
    EXPECT_EQ(answer, (std::vector<i64>{0}));
}

TEST(RerootingTest, ReceiverSideEdgeDirection) {
    NonDirectedGraph<i32> g(2);
    g.add_edge(0, 1);
    g.build();

    const auto answer = rerooting(
        g, i32{0}, [](i32 a, i32 b) { return a + b; }, [](i32 value, i32) { return value; },
        [](i32 value, const Edge<i32>& e) { return value + 10 * e.u + e.v; });
    EXPECT_EQ(answer, (std::vector<i32>{1, 10}));
}

TEST(RerootingTest, NonCommutativeNeighborOrder) {
    NonDirectedGraph<i32> g(4);
    g.add_edge(1, 2);
    g.add_edge(0, 1);
    g.add_edge(1, 3);
    g.build();

    const auto answer = rerooting(
        g, std::string{}, [](const std::string& a, const std::string& b) { return a + b; },
        [](const std::string& value, i32 v) { return "(" + std::to_string(v) + value + ")"; },
        [](const std::string& value, const Edge<i32>&) { return value; });
    EXPECT_EQ(answer[0], "(0(1(2)(3)))");
    EXPECT_EQ(answer[1], "(1(2)(0)(3))");
    EXPECT_EQ(answer[2], "(2(1(0)(3)))");
    EXPECT_EQ(answer[3], "(3(1(2)(0)))");

    const auto monoid_answer = rerooting<string_concat_monoid>(
        g, [](const std::string& value, i32 v) { return "(" + std::to_string(v) + value + ")"; },
        [](const std::string& value, const Edge<i32>&) { return value; });
    EXPECT_EQ(monoid_answer, answer);
}

TEST(RerootingTest, DeepPath) {
    constexpr i32 N = 100000;
    NonDirectedGraph<i32> g(N);
    for (i32 v = 1; v < N; ++v) g.add_edge(v - 1, v);
    g.build();

    const auto answer = rerooting(
        g, i32{0}, [](i32 a, i32 b) { return a + b; }, [](i32 value, i32) { return value + 1; },
        [](i32 value, const Edge<i32>&) { return value; });
    EXPECT_EQ(answer.front(), N);
    EXPECT_EQ(answer[N / 2], N);
    EXPECT_EQ(answer.back(), N);
}
