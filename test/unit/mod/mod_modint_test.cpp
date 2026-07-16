// clang-format off
#include "gwen/mod/modint.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen;

// コンセプトの検証
static_assert(ring<DynamicModInt64>);
static_assert(modint<DynamicModInt64>);

TEST(DynamicModInt64Test, BasicOperations) {
    DynamicModInt64::set_mod(998244353);

    DynamicModInt64 a(100);
    DynamicModInt64 b(998244352);  // -1 mod 998244353
    DynamicModInt64 c(-1);         // -1 mod 998244353

    EXPECT_EQ(b.val(), 998244352);
    EXPECT_EQ(c.val(), 998244352);

    auto d = a + b;
    EXPECT_EQ(d.val(), 99);

    d += 1;
    EXPECT_EQ(d.val(), 100);

    d -= 100;
    EXPECT_EQ(d.val(), 0);

    auto e = DynamicModInt64(2) * DynamicModInt64(3);
    EXPECT_EQ(e.val(), 6);

    auto f = DynamicModInt64(2).pow(10);
    EXPECT_EQ(f.val(), 1024);

    auto g = DynamicModInt64(2).inv();
    EXPECT_EQ((g * 2).val(), 1);

    auto h = DynamicModInt64(10) / DynamicModInt64(2);
    EXPECT_EQ(h.val(), 5);
}

TEST(DynamicModInt64Test, ModuloBehavior) {
    DynamicModInt64::set_mod(1000000007);
    EXPECT_EQ(DynamicModInt64::mod(), 1000000007);

    DynamicModInt64 a(1000000008);
    EXPECT_EQ(a.val(), 1);

    DynamicModInt64 b(-1000000008);
    EXPECT_EQ(b.val(), 1000000007 - 1);
}

TEST(ModTest, PowMod) {
    EXPECT_EQ(pow_mod(2, 10, 998244353), 1024);
    EXPECT_EQ(pow_mod(3, 3, 10), 7);
    EXPECT_EQ(pow_mod(0, 5, 10), 0);
    EXPECT_EQ(pow_mod(5, 0, 10), 1);
}

TEST(ModTest, InvMod) {
    EXPECT_EQ(inv_mod(2, 998244353) * 2 % 998244353, 1);
    EXPECT_EQ(inv_mod_64(2, 998244353) * 2 % 998244353, 1);
}
