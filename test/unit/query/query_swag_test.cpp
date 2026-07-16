#include <gtest/gtest.h>

#include <vector>

#include "gwen/query/swag.hpp"

using namespace gwen;

// Affine Transformation Monoid (Non-commutative)
// (a, b) corresponds to f(x) = ax + b
// f(g(x)) = a(cx + d) + b = acx + ad + b
// op((a, b), (c, d)) = (a*c, a*d + b)
struct AffineMonoid {
    struct S {
        i64 a, b;
        bool operator==(const S& o) const { return a == o.a && b == o.b; }
    };
    static S op(const S& l, const S& r) { return S{l.a * r.a, l.a * r.b + l.b}; }
    static S e() { return S{1, 0}; }
};

TEST(SwagDequeTest, AffineTransform) {
    SwagDeque<AffineMonoid> swag;
    swag.push_back({2, 3});
    swag.push_back({3, 4});

    // f1(x) = 2x+3, f2(x) = 3x+4
    // fold = f1(f2(x)) = 2(3x+4)+3 = 6x+11
    auto res = swag.fold();
    EXPECT_EQ(res.a, 6);
    EXPECT_EQ(res.b, 11);

    swag.push_front({4, 5});
    // f0(x) = 4x+5
    // fold = f0(f1(f2(x))) = 4(6x+11)+5 = 24x+49
    res = swag.fold();
    EXPECT_EQ(res.a, 24);
    EXPECT_EQ(res.b, 49);

    swag.pop_back();  // removes f2
    // fold = f0(f1(x)) = 4(2x+3)+5 = 8x+17
    res = swag.fold();
    EXPECT_EQ(res.a, 8);
    EXPECT_EQ(res.b, 17);

    swag.pop_front();  // removes f0
    // fold = f1(x) = 2x+3
    res = swag.fold();
    EXPECT_EQ(res.a, 2);
    EXPECT_EQ(res.b, 3);
}

TEST(SlidingWindowAggregationTest, AffineTransform) {
    i32 n = 5;
    std::vector<AffineMonoid::S> data = {{2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}};

    SlidingWindowAggregation<AffineMonoid> swag(n);
    swag.add_query(0, 2);  // 2x+3, 3x+4 -> 6x+11
    swag.add_query(1, 3);  // 3x+4, 4x+5 -> 12x+19
    swag.add_query(2, 5);  // all

    auto naive = [&](i32 l, i32 r) {
        AffineMonoid::S acc = AffineMonoid::e();
        for (i32 i = l; i < r; ++i) {
            acc = AffineMonoid::op(acc, data[i]);
        }
        return acc;
    };

    auto res = swag.solve(data, false);

    ASSERT_EQ(res.size(), 3);
    EXPECT_EQ(res[0], naive(0, 2));
    EXPECT_EQ(res[1], naive(1, 3));
    EXPECT_EQ(res[2], naive(2, 5));
}
