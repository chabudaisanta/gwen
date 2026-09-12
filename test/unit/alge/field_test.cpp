#include "gwen/alge/field.hpp"

#include <gtest/gtest.h>

#include <string>

#include "gwen/mod/mod61.hpp"
#include "gwen/mod/modint.hpp"
#include "gwen/types.hpp"

using namespace gwen;

namespace {

// 環 (Ring) ではあるが除算演算子を持たない構造体
struct NonFieldRing {
    i32 val;
    constexpr explicit NonFieldRing(i32 v) : val(v) {}

    friend constexpr NonFieldRing operator+(NonFieldRing a, NonFieldRing b) { return NonFieldRing(a.val + b.val); }
    friend constexpr NonFieldRing operator-(NonFieldRing a, NonFieldRing b) { return NonFieldRing(a.val - b.val); }
    friend constexpr NonFieldRing operator*(NonFieldRing a, NonFieldRing b) { return NonFieldRing(a.val * b.val); }
    NonFieldRing& operator+=(const NonFieldRing& o) {
        val += o.val;
        return *this;
    }
    NonFieldRing& operator-=(const NonFieldRing& o) {
        val -= o.val;
        return *this;
    }
    NonFieldRing& operator*=(const NonFieldRing& o) {
        val *= o.val;
        return *this;
    }
    NonFieldRing operator-() const { return NonFieldRing(-val); }
    bool operator==(const NonFieldRing&) const = default;
};

static_assert(ring<NonFieldRing>);
static_assert(!field<NonFieldRing>);

// 浮動小数点型
static_assert(field<f32>);
static_assert(field<f64>);

// ModInt 型
static_assert(field<DynamicModInt64>);
static_assert(field<StaticModInt64<998244353>>);
static_assert(field<ModInt61>);

// 非代数構造型
static_assert(!field<std::string>);

}  // namespace

TEST(FieldTest, FloatDivision) {
    f64 a = 6.0;
    f64 b = 2.0;
    EXPECT_DOUBLE_EQ(a / b, 3.0);
    a /= b;
    EXPECT_DOUBLE_EQ(a, 3.0);
}

TEST(FieldTest, ModIntDivision) {
    using mint = StaticModInt64<998244353>;
    mint a(6);
    mint b(2);
    EXPECT_EQ((a / b).val(), 3);
    a /= b;
    EXPECT_EQ(a.val(), 3);
}
