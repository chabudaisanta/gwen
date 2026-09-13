// clang-format off
#include "gwen/mod/modint.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <limits>

using namespace gwen;

// コンセプトの検証
static_assert(ring<DynamicModInt64>);
static_assert(modint<DynamicModInt64>);
static_assert(ring<modint998244353>);
static_assert(modint<modint998244353>);
static_assert(ring<modint1000000007>);
static_assert(modint<modint1000000007>);

TEST(StaticModInt64Test, BasicOperations) {
    modint998244353 a(100);
    modint998244353 b(998244352);  // -1 mod 998244353
    modint998244353 c(-1);         // -1 mod 998244353

    EXPECT_EQ(b.val(), 998244352);
    EXPECT_EQ(c.val(), 998244352);

    auto d = a + b;
    EXPECT_EQ(d.val(), 99);

    d += 1;
    EXPECT_EQ(d.val(), 100);

    d -= 100;
    EXPECT_EQ(d.val(), 0);

    auto e = modint998244353(2) * modint998244353(3);
    EXPECT_EQ(e.val(), 6);

    auto f = modint998244353(2).pow(10);
    EXPECT_EQ(f.val(), 1024);

    auto g = modint998244353(2).inv();
    EXPECT_EQ((g * 2).val(), 1);

    auto h = modint998244353(10) / modint998244353(2);
    EXPECT_EQ(h.val(), 5);
}

TEST(StaticModInt64Test, ModuloBehavior) {
    EXPECT_EQ(modint1000000007::mod(), 1000000007);

    modint1000000007 a(1000000008);
    EXPECT_EQ(a.val(), 1);

    modint1000000007 b(-1000000008);
    EXPECT_EQ(b.val(), 1000000007 - 1);
}

TEST(StaticModInt64Test, StorageAndIntegerBoundaries) {
    static_assert(sizeof(modint998244353) == sizeof(u32));
    constexpr u64 large_mod = 2305843009213693951ULL;
    using large_mint = StaticModInt64<large_mod>;
    static_assert(sizeof(large_mint) == sizeof(u64));

    constexpr u64 unsigned_max = std::numeric_limits<u64>::max();
    constexpr i64 signed_min = std::numeric_limits<i64>::min();
    EXPECT_EQ(modint998244353(unsigned_max).val(), unsigned_max % modint998244353::mod());
    EXPECT_EQ(modint998244353(signed_min).val(),
              static_cast<u64>((static_cast<i128>(signed_min) % modint998244353::mod() + modint998244353::mod()) %
                               modint998244353::mod()));
    EXPECT_EQ(large_mint(unsigned_max).val(), unsigned_max % large_mod);
    EXPECT_EQ(large_mint(signed_min).val(),
              static_cast<u64>((static_cast<i128>(signed_min) % large_mod + large_mod) % large_mod));

    const large_mint a(large_mod - 2);
    const large_mint b(large_mod - 3);
    EXPECT_EQ((a * b).val(), 6);
    EXPECT_EQ((a / b * b).val(), a.val());
    EXPECT_EQ(large_mint(2).pow(60).val(), (u64{1} << 60) % large_mod);
}

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
