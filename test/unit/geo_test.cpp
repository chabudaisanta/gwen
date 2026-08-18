#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "gwen/geo/convex_polygon.hpp"
#include "gwen/geo/point.hpp"
#include "gwen/types.hpp"

using namespace gwen;
using namespace gwen::geo;

TEST(GeoTest, PointBasic) {
    Point<i64> p0;  // コンストラクタの初期値挙動
    EXPECT_EQ(p0.x, 0);
    EXPECT_EQ(p0.y, 0);

    Point<i64> p1(1, 2);
    Point<i64> p2(3, 4);

    auto p3 = p1 + p2;
    EXPECT_EQ(p3.x, 4);
    EXPECT_EQ(p3.y, 6);

    auto p4 = p1 - p2;
    EXPECT_EQ(p4.x, -2);
    EXPECT_EQ(p4.y, -2);

    EXPECT_EQ(p1.dot(p2), 11);
    EXPECT_EQ(p1.cross(p2), -2);
    EXPECT_EQ(p1.norm(), 5);
}

TEST(GeoTest, PointOperators) {
    Point<i32> p1(2, 3);
    Point<i32> p2(4, 5);

    p1 += p2;
    EXPECT_EQ(p1, Point<i32>(6, 8));
    p1 -= p2;
    EXPECT_EQ(p1, Point<i32>(2, 3));

    p1 *= 2;
    EXPECT_EQ(p1, Point<i32>(4, 6));
    p1 /= 2;
    EXPECT_EQ(p1, Point<i32>(2, 3));

    EXPECT_EQ(p1 * 3, Point<i32>(6, 9));
    EXPECT_EQ(p1 / 2, Point<i32>(1, 1));

    EXPECT_TRUE(p1 == Point<i32>(2, 3));
    EXPECT_FALSE(p1 == Point<i32>(3, 2));
    EXPECT_TRUE(p1 != Point<i32>(3, 2));
    EXPECT_FALSE(p1 != Point<i32>(2, 3));
}

TEST(GeoTest, Comparators) {
    Point<i32> p1(1, 5);
    Point<i32> p2(3, 2);
    Point<i32> p3(1, 10);

    EXPECT_TRUE(CompareX{}(p1, p2));
    EXPECT_FALSE(CompareY{}(p1, p2));
    EXPECT_TRUE(CompareXy{}(p1, p3));
    EXPECT_TRUE(CompareXy{}(p1, p2));
}

TEST(GeoTest, CompareArg) {
    Point<i32> p1(1, 1);    // 第1象限
    Point<i32> p2(-1, 1);   // 第2象限
    Point<i32> p3(-1, -1);  // 第3象限
    Point<i32> p4(1, -1);   // 第4象限

    EXPECT_TRUE(CompareArg{}(p1, p2));
    EXPECT_TRUE(CompareArg{}(p2, p3));
    EXPECT_TRUE(CompareArg{}(p3, p4));

    Point<i32> p1_far(2, 2);
    EXPECT_FALSE(CompareArg{}(p1, p1_far));
    EXPECT_FALSE(CompareArg{}(p1_far, p1));

    EXPECT_TRUE(CompareArgNorm{}(p1, p1_far));
}

TEST(GeoTest, CCW) {
    Point<i32> a(0, 0);
    Point<i32> b(2, 0);

    // a -> b に対して左側
    Point<i32> c1(1, 1);
    EXPECT_EQ(ccw(a, b, c1), 1);

    // a -> b に対して右側
    Point<i32> c2(1, -1);
    EXPECT_EQ(ccw(a, b, c2), -1);

    // a -> b と逆方向
    Point<i32> c3(-1, 0);
    EXPECT_EQ(ccw(a, b, c3), 2);

    // a -> b の延長上
    Point<i32> c4(3, 0);
    EXPECT_EQ(ccw(a, b, c4), -2);

    // a -> b の線分上
    Point<i32> c5(1, 0);
    EXPECT_EQ(ccw(a, b, c5), 0);
}

TEST(GeoTest, ConvexPolygon) {
    std::vector<Point<i32>> pts = {{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}};
    auto hull = convex_polygon(pts);
    EXPECT_EQ(hull.size(), 4);
}
