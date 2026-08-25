#include "gwen/automaton/digit_dp.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "gwen/automaton/automaton.hpp"
#include "gwen/types.hpp"

using namespace gwen;

// A simple ring wrapper for testing, since built-in types might have promotion issues with concept
struct TestRing {
    i64 val;
    TestRing() : val(0) {}
    TestRing(i64 v) : val(v) {}
    TestRing operator+(const TestRing& rhs) const { return TestRing(val + rhs.val); }
    TestRing operator-(const TestRing& rhs) const { return TestRing(val - rhs.val); }
    TestRing operator*(const TestRing& rhs) const { return TestRing(val * rhs.val); }
    TestRing& operator+=(const TestRing& rhs) {
        val += rhs.val;
        return *this;
    }
    TestRing& operator-=(const TestRing& rhs) {
        val -= rhs.val;
        return *this;
    }
    TestRing& operator*=(const TestRing& rhs) {
        val *= rhs.val;
        return *this;
    }
    bool operator==(const TestRing& rhs) const { return val == rhs.val; }
};

struct MinimalDigitDpValue {
    i64 val;

    MinimalDigitDpValue(i64 value) : val(value) {}
    MinimalDigitDpValue& operator+=(const MinimalDigitDpValue& rhs) {
        val += rhs.val;
        return *this;
    }
    bool operator==(const MinimalDigitDpValue&) const = default;
};

static_assert(digit_dp_value<MinimalDigitDpValue>);

// Automaton that checks if the string does NOT contain the digit '3'.
// States: 0 (valid), 1 (invalid)
Automaton<10> build_no_3_automaton() {
    Automaton<10> a(2);
    a.init = {0};
    a.accept = {0};
    for (i32 u = 0; u < 2; ++u) {
        for (i32 c = 0; c < 10; ++c) {
            if (u == 1 || c == 3) {
                a.set_edge(u, c, 1);  // Trap state
            }
            else {
                a.set_edge(u, c, 0);  // Valid state
            }
        }
    }
    return a;
}

TEST(DigitDPTest, SmallBound) {
    auto a = build_no_3_automaton();

    // N = 45 (Length 2)
    std::vector<i32> N = {4, 5};
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    // Brute force <= 45
    i64 expected = 0;
    for (i32 i = 0; i <= 45; ++i) {
        std::string s = std::to_string(i);
        if (i < 10) s = "0" + s;
        bool ok = true;
        for (char c : s)
            if (c == '3') ok = false;
        if (ok) expected++;
    }

    EXPECT_EQ(ans.val, expected);
}

TEST(DigitDPTest, LargeBound) {
    auto a = build_no_3_automaton();

    // N = 2345 (Length 4)
    std::vector<i32> N = {2, 3, 4, 5};
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    // Brute force <= 2345
    i64 expected = 0;
    for (i32 i = 0; i <= 2345; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 4) s = "0" + s;
        bool ok = true;
        for (char c : s)
            if (c == '3') ok = false;
        if (ok) expected++;
    }

    EXPECT_EQ(ans.val, expected);
}

TEST(DigitDPTest, RequiresOnlyDigitDpValueOperations) {
    Automaton<2> a(1);
    a.init = {0};
    a.accept = {0};
    a.set_edge(0, 0, 0);
    a.set_edge(0, 1, 0);

    const auto ans = run_digit_dp<MinimalDigitDpValue, 2>({1, 1}, a);

    EXPECT_EQ(ans.val, 4);
}

TEST(DigitDPTest, DuplicateStatesActAsMultiplicity) {
    Automaton<2> a(1);
    a.init = {0, 0};
    a.accept = {0, 0};
    a.set_edge(0, 0, 0);
    a.set_edge(0, 1, 0);

    const auto ans = run_digit_dp<MinimalDigitDpValue, 2>({1}, a);

    EXPECT_EQ(ans.val, 8);
}

#ifndef NDEBUG
TEST(DigitDPTest, RejectsOutOfRangeDigit) {
    Automaton<2> a(1);
    a.init = {0};
    a.accept = {0};
    a.set_edge(0, 0, 0);
    a.set_edge(0, 1, 0);

    EXPECT_DEATH((run_digit_dp<MinimalDigitDpValue, 2>({2}, a)), "");
}
#endif
