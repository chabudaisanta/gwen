// clang-format off
#include "gwen/mod/mod61.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/ring.hpp"
#include "gwen/mod/modint.hpp"

using namespace gwen;

// コンセプトチェック
static_assert(ring<ModInt61>);
static_assert(modint<ModInt61>);

class ModInt61Test : public ::testing::Test {
protected:
    static constexpr u64 mod = (1ull << 61) - 1;
};

TEST_F(ModInt61Test, ConstructorsAndVal) {
    ModInt61 a;
    EXPECT_EQ(a.val(), 0);

    ModInt61 b(10);
    EXPECT_EQ(b.val(), 10);

    ModInt61 c(mod + 5);
    EXPECT_EQ(c.val(), 5);

    ModInt61 d(-1);
    EXPECT_EQ(d.val(), mod - 1);

    ModInt61 e(0);
    EXPECT_EQ((-e).val(), 0);
    EXPECT_EQ((-d).val(), 1);
}

TEST_F(ModInt61Test, AdditionAndSubtraction) {
    ModInt61 a(mod - 5);
    ModInt61 b(10);

    EXPECT_EQ((a + b).val(), 5);
    EXPECT_EQ((b - a).val(), 15);
    EXPECT_EQ((a - b).val(), mod - 15);

    ModInt61 c = a;
    c += 10;
    EXPECT_EQ(c.val(), 5);

    EXPECT_EQ((ModInt61(5) + 10).val(), 15);
    EXPECT_EQ((5 + ModInt61(10)).val(), 15);
}

TEST_F(ModInt61Test, MultiplicationAndDivision) {
    ModInt61 x(1ull << 32);
    ModInt61 y(1ull << 32);
    // (2^32) * (2^32) = 2^64
    // 2^64 mod (2^61 - 1) = 2^3 * 2^61 = 8 * (2^61 - 1 + 1) = 8
    EXPECT_EQ((x * y).val(), 8);

    ModInt61 inv_x = x.inv();
    EXPECT_EQ((x * inv_x).val(), 1);

    EXPECT_EQ((ModInt61(100) / 20).val(), 5);
    EXPECT_EQ((100 / ModInt61(20)).val(), 5);
}

TEST_F(ModInt61Test, Pow) {
    ModInt61 a(2);
    EXPECT_EQ(a.pow(61).val(), 1);  // 2^61 = 1 mod (2^61 - 1)
    EXPECT_EQ(a.pow(0).val(), 1);
    EXPECT_EQ(a.pow(10).val(), 1024);
}

TEST_F(ModInt61Test, Equality) {
    ModInt61 a(10), b(10), c(11);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}
