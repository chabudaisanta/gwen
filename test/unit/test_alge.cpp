#include <gtest/gtest.h>

#include "gwen/alge/abel.hpp"
#include "gwen/alge/monoid.hpp"

using namespace gwen;

// コンパイル時テスト: 各構造体が正しく concept を満たすかの検証
static_assert(monoid<sum_monoid<int>>);
static_assert(monoid<xor_monoid<long long>>);
static_assert(monoid<min_monoid<int>>);
static_assert(monoid<max_monoid<int>>);
static_assert(monoid<minmax_monoid<int>>);
static_assert(monoid<gcd_monoid<long long>>);
static_assert(monoid<affine_monoid<long long>>);

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
    auto f = affine_monoid<int>::S{2, 3}; // 2x + 3
    auto g = affine_monoid<int>::S{4, 5}; // 4x + 5
    auto fg = affine_monoid<int>::op(f, g); // f(g(x)) = 2*(4x+5)+3 = 8x+13
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
