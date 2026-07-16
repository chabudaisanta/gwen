#include "gwen/query/mo.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace gwen;

TEST(MoAlgorithmTest, RangeSum) {
    i32 n = 100;
    std::vector<i64> a(n);
    for (i32 i = 0; i < n; ++i) {
        a[i] = i + 1;
    }

    Mo mo(n);
    // Queries
    mo.add_query(0, 10);
    mo.add_query(10, 20);
    mo.add_query(0, 100);
    mo.add_query(50, 50); // empty range
    mo.add_query(20, 80);

    i64 current_sum = 0;
    
    auto add = [&](i32 idx) {
        current_sum += a[idx];
    };
    
    auto del = [&](i32 idx) {
        current_sum -= a[idx];
    };
    
    auto get_res = [&](i32 /*idx*/) {
        return current_sum;
    };

    auto res = mo.solve(add, del, get_res);

    ASSERT_EQ(res.size(), 5);

    // Compute expected results naively
    auto expected_sum = [&](i32 l, i32 r) {
        i64 sum = 0;
        for (i32 i = l; i < r; ++i) {
            sum += a[i];
        }
        return sum;
    };

    EXPECT_EQ(res[0], expected_sum(0, 10));
    EXPECT_EQ(res[1], expected_sum(10, 20));
    EXPECT_EQ(res[2], expected_sum(0, 100));
    EXPECT_EQ(res[3], expected_sum(50, 50));
    EXPECT_EQ(res[4], expected_sum(20, 80));
}

TEST(MoAlgorithmTest, FiveCallbackVersion) {
    i32 n = 100;
    std::vector<i64> a(n);
    for (i32 i = 0; i < n; ++i) {
        a[i] = i + 1;
    }

    Mo mo(n);
    mo.add_query(5, 15);
    mo.add_query(0, 50);

    i64 current_sum = 0;
    
    // decrement_l expands the left side
    auto decrement_l = [&](i32 l, i32 /*r*/) {
        current_sum += a[l];
    };
    // increment_l shrinks the left side
    auto increment_l = [&](i32 l, i32 /*r*/) {
        current_sum -= a[l];
    };
    // increment_r expands the right side
    auto increment_r = [&](i32 /*l*/, i32 r) {
        current_sum += a[r];
    };
    // decrement_r shrinks the right side
    auto decrement_r = [&](i32 /*l*/, i32 r) {
        current_sum -= a[r];
    };
    
    auto get_res = [&](i32 /*idx*/) {
        return current_sum;
    };

    auto res = mo.solve(increment_l, decrement_l, increment_r, decrement_r, get_res);

    ASSERT_EQ(res.size(), 2);
    
    auto expected_sum = [&](i32 l, i32 r) {
        i64 sum = 0;
        for (i32 i = l; i < r; ++i) {
            sum += a[i];
        }
        return sum;
    };

    EXPECT_EQ(res[0], expected_sum(5, 15));
    EXPECT_EQ(res[1], expected_sum(0, 50));
}
