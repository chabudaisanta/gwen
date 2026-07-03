#include <gtest/gtest.h>
#include <vector>
#include "gwen/algebra/basic_ring.hpp"
#include "gwen/query/polynomial_difference_array.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(PolynomialDifferenceArrayTest, BasicAddition) {
    i32 n = 10;
    i32 k = 2;
    PolynomialDifferenceArray<sum_ring<i64>> imos(n, k);

    // 1. [2, 6) に多項式 f(i) = 1 + 2*i を加算
    std::vector<i64> poly = {1, 2};
    imos.add_poly(2, 6, poly);

    // 2. [4, 8) に単一の項 3*i^2 を加算
    imos.add_term(4, 8, 3, 2);

    auto res = imos.build();

    std::vector<i64> expected(n, 0);

    for (i32 i = 2; i < 6; ++i) {
        expected[i] += 1 + 2 * i;
    }
    for (i32 i = 4; i < 8; ++i) {
        expected[i] += 3 * i * i;
    }

    EXPECT_EQ(res, expected);
}

TEST(PolynomialDifferenceArrayTest, EdgeCases) {
    i32 n = 5;
    i32 k = 1;
    PolynomialDifferenceArray<sum_ring<i64>> imos(n, k);

    // 範囲外の区間や、空の区間を加算
    imos.add_poly(0, 0, std::vector<i64>{100}); // 空区間
    imos.add_poly(3, 10, std::vector<i64>{10}); // 右側がはみ出る
    imos.add_term(-5, 2, 5, 0); // 左側がはみ出る
}

TEST(PolynomialDifferenceArrayTest, OutOfBoundsRight) {
    i32 n = 5;
    PolynomialDifferenceArray<sum_ring<i64>> imos(n, 1);
    
    // R が n より大きい場合
    imos.add_term(2, 10, 5, 0); // [2, 5) のみに加算されるはず
    auto res = imos.build();
    
    std::vector<i64> expected = {0, 0, 5, 5, 5};
    EXPECT_EQ(res, expected);
}
