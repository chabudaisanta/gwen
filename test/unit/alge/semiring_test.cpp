// clang-format off
#include "gwen/alge/ring.hpp"
#include "gwen/alge/semiring.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <string>

#include "gwen/types.hpp"

using namespace gwen;

namespace {

struct TestSemiring {
    i32 value;

    constexpr explicit TestSemiring(i32 value_) : value(value_) {}

    friend constexpr TestSemiring operator+(TestSemiring a, TestSemiring b) { return TestSemiring(a.value + b.value); }

    friend constexpr TestSemiring operator*(TestSemiring a, TestSemiring b) { return TestSemiring(a.value * b.value); }
};

static_assert(semiring<i32>);
static_assert(ring<i32>);
static_assert(semiring<TestSemiring>);
static_assert(!ring<TestSemiring>);
static_assert(!semiring<std::string>);

}  // namespace

TEST(SemiringTest, Operations) {
    const TestSemiring a(2);
    const TestSemiring b(3);
    EXPECT_EQ((a + b).value, 5);
    EXPECT_EQ((a * b).value, 6);
    EXPECT_EQ(TestSemiring(0).value, 0);
    EXPECT_EQ(TestSemiring(1).value, 1);
}
