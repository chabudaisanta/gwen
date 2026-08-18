#include "gwen/automaton/factory.hpp"

#include <gtest/gtest.h>

#include <span>
#include <string>
#include <vector>

#include "gwen/automaton/digit_dp.hpp"
#include "gwen/types.hpp"

using namespace gwen;
using namespace gwen::automaton;

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

TEST(AutomatonFactoryTest, DigitSumExact) {
    auto a = digit_sum_exact<10>(12);
    std::vector<i32> N = {4, 9, 2};  // N = 492
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 492; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 3) s = "0" + s;
        i32 sum = 0;
        for (char c : s) sum += c - '0';
        if (sum == 12) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}

TEST(AutomatonFactoryTest, DigitSumLeq) {
    auto a = digit_sum_leq<10>(5);
    std::vector<i32> N = {1, 2, 3};  // N = 123
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 123; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 3) s = "0" + s;
        i32 sum = 0;
        for (char c : s) sum += c - '0';
        if (sum <= 5) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}

TEST(AutomatonFactoryTest, DigitSumMod) {
    auto a = digit_sum_mod<10>(7, 3);
    std::vector<i32> N = {5, 0, 0};  // 500
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 500; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 3) s = "0" + s;
        i32 sum = 0;
        for (char c : s) sum += c - '0';
        if (sum % 7 == 3) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}

TEST(AutomatonFactoryTest, ExcludeDigits) {
    std::vector<i32> S = {4, 9};
    auto a = exclude_digits<10>(S);
    std::vector<i32> N = {1, 0, 0, 0};  // 1000
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 1000; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 4) s = "0" + s;
        bool ok = true;
        for (char c : s)
            if (c == '4' || c == '9') ok = false;
        if (ok) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}

TEST(AutomatonFactoryTest, ValueMod) {
    auto a = value_mod<10>(13, 5);
    std::vector<i32> N = {2, 0, 0, 0};  // 2000
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 2000; ++i) {
        if (i % 13 == 5) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}

TEST(AutomatonFactoryTest, NonZeroCountExact) {
    auto a = non_zero_count_exact<10>(2);
    std::vector<i32> N = {3, 0, 0, 0};  // 3000
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 3000; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 4) s = "0" + s;
        i32 cnt = 0;
        for (char c : s)
            if (c != '0') cnt++;
        if (cnt == 2) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}

TEST(AutomatonFactoryTest, NonZeroCountLeq) {
    auto a = non_zero_count_leq<10>(1);
    std::vector<i32> N = {2, 0, 0};  // 200
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 200; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 3) s = "0" + s;
        i32 cnt = 0;
        for (char c : s)
            if (c != '0') cnt++;
        if (cnt <= 1) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}



TEST(AutomatonFactoryTest, IncludeAllDigits) {
    std::vector<i32> S = {1, 3, 3}; // 3 is duplicated
    auto a = include_all_digits<10>(S);
    std::vector<i32> N = {3, 0, 0};  // 300
    auto ans = run_digit_dp<TestRing, 10>(N, a);

    i64 expected = 0;
    for (i32 i = 0; i <= 300; ++i) {
        std::string s = std::to_string(i);
        while (s.length() < 3) s = "0" + s;
        bool has_1 = false;
        bool has_3 = false;
        for (char c : s) {
            if (c == '1') has_1 = true;
            if (c == '3') has_3 = true;
        }
        if (has_1 && has_3) expected++;
    }
    EXPECT_EQ(ans.val, expected);
}
