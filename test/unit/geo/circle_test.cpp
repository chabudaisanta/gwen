#include "gwen/geo/circle.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <random>

#include "gwen/types.hpp"

using namespace gwen;
using namespace gwen::geo;

template <std::floating_point T>
void expect_on_circles(const Circle<T>& a, const Circle<T>& b, const std::array<Point<T>, 2>& points, T tolerance) {
    for (const auto& p : points) {
        EXPECT_NEAR(distance(p, a.center), a.radius, tolerance);
        EXPECT_NEAR(distance(p, b.center), b.radius, tolerance);
    }
    EXPECT_FALSE(CompareXy{}(points[1], points[0]));
}

TEST(CircleTest, TwoIntersectionsAndOrder) {
    const Circle<f64> a({0, 0}, 2), b({2, 0}, 2);
    const auto [count, points] = intersection(a, b);
    ASSERT_EQ(count, 2);
    EXPECT_DOUBLE_EQ(points[0].x, 1);
    EXPECT_DOUBLE_EQ(points[1].x, 1);
    EXPECT_NEAR(points[0].y, -std::sqrt(3.0), 1e-12);
    EXPECT_NEAR(points[1].y, std::sqrt(3.0), 1e-12);
    expect_on_circles(a, b, points, 1e-12);
    const auto reversed = intersection(b, a);
    ASSERT_EQ(reversed.first, 2);
    EXPECT_EQ(reversed.second, points);
    const Circle<f80> vertical({0, 2}, 2), origin({0, 0}, 2);
    const auto v = intersection(origin, vertical);
    ASSERT_EQ(v.first, 2);
    EXPECT_LT(v.second[0].x, v.second[1].x);
    expect_on_circles(origin, vertical, v.second, 1e-15L);
    const Circle<f64> diagonal({3, 4}, 4);
    const auto d = intersection(a, diagonal);
    ASSERT_EQ(d.first, 2);
    expect_on_circles(a, diagonal, d.second, 1e-12);
}

TEST(CircleTest, NoIntersectionAndCoincidence) {
    const Circle<f64> a({0, 0}, 2);
    EXPECT_EQ(intersection(a, Circle<f64>({5, 0}, 2)).first, 0);
    EXPECT_EQ(intersection(a, Circle<f64>({0.25, 0}, 1)).first, 0);
    EXPECT_EQ(intersection(a, Circle<f64>({0, 0}, 1)).first, 0);
    EXPECT_EQ(intersection(a, Circle<f64>({0, 0}, 1), 100.0).first, 0);
    EXPECT_EQ(intersection(a, a).first, -1);
}

TEST(CircleTest, Tangency) {
    const Circle<f64> a({0, 0}, 2);
    const auto external = intersection(a, Circle<f64>({3, 0}, 1));
    ASSERT_EQ(external.first, 1);
    EXPECT_EQ(external.second[0], Point<f64>(2, 0));
    const Circle<f64> b({1, 0}, 1);
    const auto internal = intersection(a, b);
    ASSERT_EQ(internal.first, 1);
    EXPECT_EQ(internal.second[0], Point<f64>(2, 0));
    const auto reversed = intersection(b, a);
    ASSERT_EQ(reversed.first, 1);
    EXPECT_EQ(reversed.second[0], internal.second[0]);
}

TEST(CircleTest, ZeroRadius) {
    const Circle<f64> p({0, 0}, 0), q({1, 0}, 0);
    EXPECT_EQ(intersection(p, p).first, 1);
    EXPECT_EQ(intersection(p, p).second[0], p.center);
    EXPECT_EQ(intersection(p, q).first, 0);
    EXPECT_EQ(intersection(p, q, 2.0).first, 1);
    const Circle<f64> a({1, 0}, 1);
    EXPECT_EQ(intersection(p, a).first, 1);
    EXPECT_EQ(intersection(a, p).second[0], p.center);
    EXPECT_EQ(intersection(p, Circle<f64>({0, 0}, 1), 2.0).first, 0);
    EXPECT_EQ(intersection(p, Circle<f64>({3, 0}, 1)).first, 0);
    EXPECT_EQ(intersection(p, Circle<f64>({1.125, 0}, 1), 0.125).first, 1);
    EXPECT_EQ(intersection(p, Circle<f64>({1.125, 0}, 1), 0.0625).first, 0);
}

TEST(CircleTest, ToleranceOnlyRelaxesNonIntersection) {
    const Circle<f64> a({0, 0}, 2), b({3.125, 0}, 1);
    const auto external = intersection(a, b, 0.125);
    ASSERT_EQ(external.first, 1);
    EXPECT_EQ(external.second[0], Point<f64>(2.0625, 0));
    EXPECT_NEAR(std::abs(distance(external.second[0], a.center) - a.radius), 0.0625, 1e-15);
    EXPECT_NEAR(std::abs(distance(external.second[0], b.center) - b.radius), 0.0625, 1e-15);
    EXPECT_EQ(intersection(a, b, 0.0625).first, 0);
    const Circle<f64> c({0.875, 0}, 1);
    const auto internal = intersection(a, c, 0.125);
    ASSERT_EQ(internal.first, 1);
    EXPECT_EQ(internal.second[0], Point<f64>(1.9375, 0));
    EXPECT_EQ(intersection(c, a, 0.125).second[0], internal.second[0]);
    EXPECT_EQ(intersection(a, c, 0.0625).first, 0);
    EXPECT_EQ(intersection(a, Circle<f64>({2.999, 0}, 1), 1.0).first, 2);
    const Circle<f64> near({1e-12, 0}, 2);
    const auto result = intersection(a, near);
    ASSERT_EQ(result.first, 2);
    expect_on_circles(a, near, result.second, 1e-12);
    const Circle<f64> unequal({2e-12, 0}, 2 - 1e-12);
    const auto other = intersection(a, unequal);
    ASSERT_EQ(other.first, 2);
    expect_on_circles(a, unequal, other.second, 1e-12);
}

TEST(CircleTest, ScalesAndRandomCircleResiduals) {
    for (f64 scale : {1e-9, 1.0, 1e9}) {
        const Circle<f64> a({2 * scale, 3 * scale}, 2 * scale), b({4 * scale, 3 * scale}, 2 * scale);
        const auto result = intersection(a, b, 0.0);
        ASSERT_EQ(result.first, 2);
        expect_on_circles(a, b, result.second, scale * 1e-12);
    }
    std::mt19937_64 rng(20260929);
    std::uniform_real_distribution<f64> coordinate(-100, 100), radius(1, 20), fraction(0.1, 0.9), angle(-3, 3);
    for (i32 i = 0; i < 1000; ++i) {
        const f64 r1 = radius(rng), r2 = radius(rng);
        const f64 d = std::abs(r1 - r2) + 2 * std::min(r1, r2) * fraction(rng);
        const f64 theta = angle(rng);
        const Circle<f64> a({coordinate(rng), coordinate(rng)}, r1);
        const Circle<f64> b(a.center + Point<f64>(d * std::cos(theta), d * std::sin(theta)), r2);
        const auto result = intersection(a, b, 0.0);
        ASSERT_EQ(result.first, 2);
        expect_on_circles(a, b, result.second, 1e-10);
        const auto reversed = intersection(b, a, 0.0);
        ASSERT_EQ(reversed.first, 2);
        for (usize j = 0; j < 2; ++j) EXPECT_LT(distance(result.second[j], reversed.second[j]), 1e-10);
    }
}

#ifndef NDEBUG
TEST(CircleDeathTest, InvalidContracts) {
    EXPECT_DEATH((Circle<f64>({0, 0}, -1)), "radius");
    const f64 inf = std::numeric_limits<f64>::infinity();
    const f64 nan = std::numeric_limits<f64>::quiet_NaN();
    EXPECT_DEATH((Circle<f64>({inf, 0}, 1)), "isfinite");
    EXPECT_DEATH((Circle<f64>({0, 0}, nan)), "radius");
    Circle<f64> a({0, 0}, 1), b({1, 0}, 1);
    EXPECT_DEATH(intersection(a, b, -1.0), "eps");
    EXPECT_DEATH(intersection(a, b, inf), "eps");
    a.radius = -1;
    EXPECT_DEATH(intersection(a, b), "radius");
    EXPECT_DEATH(intersection(b, a), "radius");
    a.radius = 1;
    a.center.y = nan;
    EXPECT_DEATH(intersection(a, b), "isfinite");
}
#endif
