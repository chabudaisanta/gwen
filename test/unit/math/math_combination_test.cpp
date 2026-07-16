// clang-format off
#include "gwen/math/combination.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/mod/mod61.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;

// Setup a dynamic modint for testing
using mint = DynamicModInt64;

TEST(CombinationTest, BasicOperations) {
    mint::set_mod(998244353);
    Combination<mint> comb(10);

    EXPECT_EQ(comb.fact(0).val(), 1);
    EXPECT_EQ(comb.fact(1).val(), 1);
    EXPECT_EQ(comb.fact(2).val(), 2);
    EXPECT_EQ(comb.fact(3).val(), 6);
    EXPECT_EQ(comb.fact(4).val(), 24);
    EXPECT_EQ(comb.fact(5).val(), 120);

    EXPECT_EQ(comb.fact_inv(0).val(), 1);
    EXPECT_EQ(comb.fact_inv(1).val(), 1);
    EXPECT_EQ(comb.fact_inv(2).val(), mint(2).inv().val());
    EXPECT_EQ(comb.fact_inv(3).val(), mint(6).inv().val());
    EXPECT_EQ(comb.fact_inv(4).val(), mint(24).inv().val());

    EXPECT_EQ(comb.comb(5, 0).val(), 1);
    EXPECT_EQ(comb.comb(5, 1).val(), 5);
    EXPECT_EQ(comb.comb(5, 2).val(), 10);
    EXPECT_EQ(comb.comb(5, 3).val(), 10);
    EXPECT_EQ(comb.comb(5, 4).val(), 5);
    EXPECT_EQ(comb.comb(5, 5).val(), 1);

    EXPECT_EQ(comb.comb(5, -1).val(), 0);
    EXPECT_EQ(comb.comb(5, 6).val(), 0);

    EXPECT_EQ(comb.perm(5, 0).val(), 1);
    EXPECT_EQ(comb.perm(5, 1).val(), 5);
    EXPECT_EQ(comb.perm(5, 2).val(), 20);
    EXPECT_EQ(comb.perm(5, 3).val(), 60);
    EXPECT_EQ(comb.perm(5, 4).val(), 120);
    EXPECT_EQ(comb.perm(5, 5).val(), 120);
}

TEST(CombinationTest, DynamicExpansion) {
    mint::set_mod(998244353);
    Combination<mint> comb(1);  // Starts very small

    EXPECT_EQ(comb.comb(10, 3).val(), 120);
    // 10C3 triggered expansion!

    EXPECT_EQ(comb.fact(10).val(), 3628800);
}

TEST(CombinationTest, DifferentModulus) {
    // 1st instance
    mint::set_mod(7);
    Combination<mint> comb1(10);
    EXPECT_EQ(comb1.fact(3).val(), 6);
    EXPECT_EQ(comb1.comb(5, 2).val(), 3);  // 10 % 7 = 3

    // 2nd instance with different mod
    mint::set_mod(11);
    Combination<mint> comb2(10);
    EXPECT_EQ(comb2.fact(3).val(), 6);
    EXPECT_EQ(comb2.comb(5, 2).val(), 10);  // 10 % 11 = 10
}

TEST(CombinationTest, Mod61Support) {
    Combination<ModInt61> comb(10);
    EXPECT_EQ(comb.comb(5, 2).val(), 10);
}

TEST(CombinationTest, ModClip) {
    mint::set_mod(7);
    Combination<mint> comb(1);

    // Try to get fact(6). It should expand, but new_size should clip to 7.
    // If it expands to 8, it would divide by zero.
    EXPECT_EQ(comb.fact(6).val(), 720 % 7);
}
