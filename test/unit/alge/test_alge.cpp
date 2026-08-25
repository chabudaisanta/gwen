// clang-format off
#include "gwen/alge/abel.hpp"
#include "gwen/alge/monoid.hpp"
// clang-format on
#include <gtest/gtest.h>

#include <limits>

using namespace gwen;

// コンパイル時テスト: 各構造体が正しく concept を満たすかの検証
static_assert(monoid<sum_monoid<int>>);
static_assert(monoid<xor_monoid<long long>>);
static_assert(monoid<min_monoid<int>>);
static_assert(monoid<max_monoid<int>>);
static_assert(monoid<minmax_monoid<int>>);
static_assert(monoid<gcd_monoid<long long>>);
static_assert(monoid<affine_monoid<long long>>);
static_assert(monoid<mul_monoid<int>>);
static_assert(monoid<and_monoid<int>>);
static_assert(monoid<no_op_monoid>);

static_assert(abel<sum_abel<int>>);
static_assert(abel<xor_abel<long long>>);

TEST(AlgeTest, MonoidOpAndE) {
    // sum
    EXPECT_EQ(sum_monoid<int>::e(), 0);
    EXPECT_EQ(sum_monoid<int>::op(3, 5), 8);

    // min
    EXPECT_EQ(min_monoid<int>::e(), std::numeric_limits<int>::max());
    EXPECT_EQ(min_monoid<int>::op(3, 5), 3);

    // max
    EXPECT_EQ(max_monoid<int>::e(), std::numeric_limits<int>::min());
    EXPECT_EQ(max_monoid<int>::op(3, 5), 5);

    // minmax
    auto minmax_e = minmax_monoid<int>::e();
    EXPECT_EQ(minmax_e.min, std::numeric_limits<int>::max());
    EXPECT_EQ(minmax_e.max, std::numeric_limits<int>::min());

    auto minmax_res = minmax_monoid<int>::op({3, 10}, {5, 8});
    EXPECT_EQ(minmax_res.min, 3);
    EXPECT_EQ(minmax_res.max, 10);

    // gcd
    EXPECT_EQ(gcd_monoid<int>::e(), 0);
    EXPECT_EQ(gcd_monoid<int>::op(12, 18), 6);

    // affine (ax + b)
    auto f = affine_monoid<int>::S{2, 3};    // 2x + 3
    auto g = affine_monoid<int>::S{4, 5};    // 4x + 5
    auto fg = affine_monoid<int>::op(f, g);  // f(g(x)) = 2*(4x+5)+3 = 8x+13
    EXPECT_EQ(fg.a, 8);
    EXPECT_EQ(fg.b, 13);

    auto affine_e = affine_monoid<int>::e();
    EXPECT_EQ(affine_e.a, 1);
    EXPECT_EQ(affine_e.b, 0);
}

TEST(AlgeTest, AbelInv) {
    // sum abel
    EXPECT_EQ(sum_abel<int>::inv(5), -5);
    EXPECT_EQ(sum_abel<int>::op(5, sum_abel<int>::inv(5)), sum_abel<int>::e());

    // xor abel
    EXPECT_EQ(xor_abel<int>::inv(5), 5);
    EXPECT_EQ(xor_abel<int>::op(5, xor_abel<int>::inv(5)), xor_abel<int>::e());
}

TEST(AlgeTest, DoubleTypes) {
    // max_monoid<double>
    EXPECT_EQ(max_monoid<double>::e(), -std::numeric_limits<double>::infinity());
    EXPECT_LT(max_monoid<double>::e(), 0.0);
    EXPECT_EQ(max_monoid<double>::op(-3.14, -5.5), -3.14);

    // minmax_monoid<double>
    auto minmax_e = minmax_monoid<double>::e();
    EXPECT_EQ(minmax_e.min, std::numeric_limits<double>::infinity());
    EXPECT_EQ(minmax_e.max, -std::numeric_limits<double>::infinity());

    auto minmax_res = minmax_monoid<double>::op({3.5, 10.1}, {5.0, 8.8});
    EXPECT_EQ(minmax_res.min, 3.5);
    EXPECT_EQ(minmax_res.max, 10.1);
}

TEST(AlgeTest, AndMonoid) {
    EXPECT_EQ(and_monoid<i32>::op(and_monoid<i32>::e(), 0x12345678), 0x12345678);
    EXPECT_EQ(and_monoid<i32>::op(0x12345678, and_monoid<i32>::e()), 0x12345678);
    EXPECT_EQ(and_monoid<i32>::op(0b1100, 0b1010), 0b1000);

    EXPECT_EQ(and_monoid<u32>::op(and_monoid<u32>::e(), 0x89abcdefU), 0x89abcdefU);
    EXPECT_EQ(and_monoid<u32>::op(0x89abcdefU, and_monoid<u32>::e()), 0x89abcdefU);
}

TEST(AlgeTest, NoOpMonoid) {
    const no_op_monoid::S x;
    const no_op_monoid::F f;
    EXPECT_EQ(no_op_monoid::op(no_op_monoid::e(), x), x);
    EXPECT_EQ(no_op_monoid::op(x, no_op_monoid::e()), x);
    EXPECT_EQ(no_op_monoid::op(no_op_monoid::op(x, x), x), no_op_monoid::op(x, no_op_monoid::op(x, x)));
    EXPECT_EQ(no_op_monoid::mapping(no_op_monoid::id(), x), x);
    EXPECT_EQ(no_op_monoid::composition(f, no_op_monoid::id()), f);
}

TEST(AlgeTest, FloatingPointInfinityIdentity) {
    constexpr double inf = std::numeric_limits<double>::infinity();

    EXPECT_EQ(min_monoid<double>::op(min_monoid<double>::e(), inf), inf);
    EXPECT_EQ(min_monoid<double>::op(inf, min_monoid<double>::e()), inf);
    EXPECT_EQ(max_monoid<double>::op(max_monoid<double>::e(), -inf), -inf);
    EXPECT_EQ(max_monoid<double>::op(-inf, max_monoid<double>::e()), -inf);

    const auto minmax_e = minmax_monoid<double>::e();
    const minmax_monoid<double>::S positive_infinity{inf, inf};
    const minmax_monoid<double>::S negative_infinity{-inf, -inf};
    const auto positive_left = minmax_monoid<double>::op(minmax_e, positive_infinity);
    const auto positive_right = minmax_monoid<double>::op(positive_infinity, minmax_e);
    const auto negative_left = minmax_monoid<double>::op(minmax_e, negative_infinity);
    const auto negative_right = minmax_monoid<double>::op(negative_infinity, minmax_e);
    EXPECT_EQ(positive_left.min, inf);
    EXPECT_EQ(positive_left.max, inf);
    EXPECT_EQ(positive_right.min, inf);
    EXPECT_EQ(positive_right.max, inf);
    EXPECT_EQ(negative_left.min, -inf);
    EXPECT_EQ(negative_left.max, -inf);
    EXPECT_EQ(negative_right.min, -inf);
    EXPECT_EQ(negative_right.max, -inf);
}
