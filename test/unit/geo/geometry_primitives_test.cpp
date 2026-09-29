#include <gtest/gtest.h>

#include <limits>

#include "gwen/geo/line.hpp"
#include "gwen/geo/segment.hpp"
#include "gwen/types.hpp"

using namespace gwen;
using namespace gwen::geo;

TEST(GeometryPrimitivesTest, PointDistance) {
    EXPECT_DOUBLE_EQ(distance(Point<f64>(1, 2), Point<f64>(4, 6)), 5);
    EXPECT_DOUBLE_EQ(distance(Point<f64>(1, 2), Point<f64>(1, 2)), 0);
    EXPECT_NEAR(distance(Point<f80>(0, 0), Point<f80>(3, 4)), 5, 1e-15L);
}

TEST(GeometryPrimitivesTest, LineProjectionAndDistance) {
    const Line<f64> horizontal({0, 0}, {2, 0});
    EXPECT_EQ(horizontal.projection({-3, 4}), Point<f64>(-3, 0));
    EXPECT_DOUBLE_EQ(horizontal.distance({-3, 4}), 4);
    const Line<f64> vertical({2, 1}, {2, -5});
    EXPECT_EQ(vertical.projection({-3, 4}), Point<f64>(2, 4));
    EXPECT_DOUBLE_EQ(vertical.distance({-3, 4}), 5);
    const Line<f80> diagonal({0, 0}, {3, 4});
    const auto p = diagonal.projection({2, 5});
    EXPECT_NEAR(p.x, 3.12L, 1e-15L);
    EXPECT_NEAR(p.y, 4.16L, 1e-15L);
    EXPECT_NEAR(diagonal.distance({2, 5}), 1.4L, 1e-15L);
}

TEST(GeometryPrimitivesTest, SegmentClosestPoint) {
    const Segment<f64> s({0, 0}, {2, 0});
    EXPECT_EQ(s.projection({1, 3}), Point<f64>(1, 0));
    EXPECT_EQ(s.projection({-1, 3}), s.a);
    EXPECT_EQ(s.projection({3, 3}), s.b);
    EXPECT_DOUBLE_EQ(s.distance({1, 3}), 3);
    EXPECT_NEAR(s.distance({-1, 3}), std::sqrt(10.0), 1e-12);
    const Segment<f80> diagonal({1, 1}, {4, 5});
    EXPECT_EQ(diagonal.projection({100, 100}), diagonal.b);
    const Segment<f64> point({1, 2}, {1, 2});
    EXPECT_EQ(point.projection({10, 20}), point.a);
    EXPECT_DOUBLE_EQ(point.distance({4, 6}), 5);
}

TEST(GeometryPrimitivesTest, SegmentIntersection) {
    const Segment<f64> s({0, 0}, {3, 0});
    EXPECT_TRUE(s.intersects(Segment<f64>({1, 1}, {2, -1}), 0.0));
    EXPECT_TRUE(s.intersects(Segment<f64>({3, 1}, {3, -1}), 0.0));
    EXPECT_TRUE(s.intersects(Segment<f64>({1, 0}, {4, 0}), 0.0));
    EXPECT_TRUE(s.intersects(Segment<f64>({3, 0}, {4, 1}), 0.0));
    EXPECT_FALSE(s.intersects(Segment<f64>({3, -2}, {5, 0}), 0.0));
    EXPECT_FALSE(s.intersects(Segment<f64>({4, 0}, {5, 0}), 0.0));
    EXPECT_FALSE(s.intersects(Segment<f64>({0, 1}, {3, 1}), 0.0));
    const Segment<f64> point({1, 0}, {1, 0});
    EXPECT_TRUE(s.intersects(point, 0.0));
    EXPECT_TRUE(point.intersects(s, 0.0));
    EXPECT_TRUE(point.intersects(point, 0.0));
    EXPECT_FALSE(point.intersects(Segment<f64>({1, 1}, {1, 1}), 0.0));
    const Segment<f80> t({0, 0}, {3, 4});
    EXPECT_TRUE(t.intersects(Segment<f80>({0, 4}, {3, 0}), 0.0L));
}

TEST(GeometryPrimitivesTest, SegmentTolerance) {
    const Segment<f64> s({0, 0}, {2, 0});
    const Segment<f64> parallel({0, 0.25}, {2, 0.25});
    EXPECT_TRUE(s.intersects(parallel, 0.25));
    EXPECT_TRUE(parallel.intersects(s, 0.25));
    EXPECT_FALSE(s.intersects(parallel, 0.125));
    const Segment<f64> point({2.25, 0}, {2.25, 0});
    EXPECT_TRUE(s.intersects(point, 0.25));
    EXPECT_FALSE(s.intersects(point, 0.125));
}

TEST(GeometryPrimitivesTest, DiagonalCollinearAndPointSegmentsWithoutTolerance) {
    const Segment<f64> s({0, 0}, {1, 1});
    const Segment<f64> overlap({0.25, 0.25}, {0.75, 0.75});
    const Segment<f64> point({0.5, 0.5}, {0.5, 0.5});
    EXPECT_TRUE(s.intersects(overlap, 0.0));
    EXPECT_TRUE(overlap.intersects(s, 0.0));
    EXPECT_TRUE(s.intersects(point, 0.0));
    EXPECT_TRUE(point.intersects(s, 0.0));
    EXPECT_TRUE(s.intersects(Segment<f64>(s.b, {2, 2}), 0.0));
    EXPECT_FALSE(s.intersects(Segment<f64>({1.25, 1.25}, {2, 2}), 0.0));
    EXPECT_FALSE(s.intersects(Segment<f64>({0.5, 0.25}, {0.5, 0.25}), 0.0));
    EXPECT_EQ(s.projection(s.a), s.a);
    EXPECT_EQ(s.projection(s.b), s.b);
}

#ifndef NDEBUG
TEST(GeometryPrimitivesDeathTest, InvalidContracts) {
    EXPECT_DEATH((Line<f64>({1, 2}, {1, 2})), "a != b");
    Line<f64> line({0, 0}, {1, 0});
    line.b = line.a;
    EXPECT_DEATH(line.projection({2, 3}), "a != b");
    EXPECT_DEATH(line.distance({2, 3}), "a != b");
    const f64 inf = std::numeric_limits<f64>::infinity();
    const f64 nan = std::numeric_limits<f64>::quiet_NaN();
    EXPECT_DEATH((Segment<f64>({inf, 0}, {0, 0})), "isfinite");
    Segment<f64> s({0, 0}, {1, 1});
    EXPECT_DEATH(s.intersects(s, -1.0), "eps");
    EXPECT_DEATH(s.intersects(s, nan), "eps");
    s.b.x = inf;
    EXPECT_DEATH(s.projection({0, 0}), "isfinite");
    EXPECT_DEATH(distance(Point<f64>(nan, 0), Point<f64>(0, 0)), "isfinite");
}
#endif
