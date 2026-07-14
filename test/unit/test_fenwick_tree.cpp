#include <gtest/gtest.h>

#include <vector>

#include "gwen/types.hpp"
#include "gwen/alge/abel.hpp"
#include "gwen/ds/fenwick_tree.hpp"

#include "testlib.h"
#include "utils/naive_array.hpp"
#include "utils/random_seed.hpp"

using namespace gwen;

TEST(FenwickTreeTest, SumAbel) {
    FenwickTree<sum_abel<i64>> bit(5);
    // [0, 0, 0, 0, 0]
    bit.add(0, 1);
    bit.add(1, 2);
    bit.add(2, 3);
    bit.add(3, 4);
    bit.add(4, 5);
    // array: [1, 2, 3, 4, 5]

    EXPECT_EQ(bit.sum(0), 0);
    EXPECT_EQ(bit.sum(1), 1);
    EXPECT_EQ(bit.sum(2), 3);
    EXPECT_EQ(bit.sum(3), 6);
    EXPECT_EQ(bit.sum(4), 10);
    EXPECT_EQ(bit.sum(5), 15);

    EXPECT_EQ(bit.sum(0, 5), 15);
    EXPECT_EQ(bit.sum(1, 3), 5);  // 2 + 3
    EXPECT_EQ(bit.sum(2, 5), 12); // 3 + 4 + 5
}

TEST(FenwickTreeTest, XorAbel) {
    std::vector<i32> init = {1, 2, 3, 4, 5};
    FenwickTree<xor_abel<i32>> bit(init);

    EXPECT_EQ(bit.sum(1), 1);
    EXPECT_EQ(bit.sum(2), 1 ^ 2);
    EXPECT_EQ(bit.sum(3), 1 ^ 2 ^ 3);
    EXPECT_EQ(bit.sum(4), 1 ^ 2 ^ 3 ^ 4);
    EXPECT_EQ(bit.sum(5), 1 ^ 2 ^ 3 ^ 4 ^ 5);

    EXPECT_EQ(bit.sum(1, 4), 2 ^ 3 ^ 4); // 5

    bit.add(2, 7); // a[2] (which was 3) ^= 7 -> 4
    // array is now {1, 2, 4, 4, 5}
    EXPECT_EQ(bit.sum(2, 3), 4);
    EXPECT_EQ(bit.sum(1, 4), 2 ^ 4 ^ 4); // 2
}

TEST(FenwickTreeTest, MaxRight) {
    FenwickTree<sum_abel<i32>> bit(5);
    bit.add(0, 1);
    bit.add(1, 2);
    bit.add(2, 3);
    bit.add(3, 4);
    bit.add(4, 5);
    // prefix sums: 0, 1, 3, 6, 10, 15

    // max_right(f) returns max r st f(sum(0, r)) is true
    EXPECT_EQ(bit.max_right([](i32 x) { return x <= 6; }), 3);
    EXPECT_EQ(bit.max_right([](i32 x) { return x <= 2; }), 1);
    EXPECT_EQ(bit.max_right([](i32 x) { return x <= 15; }), 5);
    EXPECT_EQ(bit.max_right([](i32 x) { return x <= 16; }), 5);
    EXPECT_EQ(bit.max_right([](i32 x) { return x <= 0; }), 0);

    // max_right(l, f)
    // sum(1, r) <= 5
    // r=1: sum=0 <= 5 (T)
    // r=2: sum=2 <= 5 (T)
    // r=3: sum=5 <= 5 (T)
    // r=4: sum=9 <= 5 (F)
    EXPECT_EQ(bit.max_right(1, [](i32 x) { return x <= 5; }), 3);

    // sum(2, r) <= 2
    // r=2: sum=0 <= 2 (T)
    // r=3: sum=3 <= 2 (F)
    EXPECT_EQ(bit.max_right(2, [](i32 x) { return x <= 2; }), 2);
}

TEST(FenwickTreeTest, RandomTest) {
    test::setup_random_seed();
    i32 T = 100;
    const i32 MAX_N = 40, MIN_N = 5;
    const i32 MAX_Q = 40, MIN_Q = 0;
    const i64 MAX_A = 1001001001, MIN_A = -1001001001;
    while(T--) {
        i32 N = rnd.next(MIN_N, MAX_N);
        FenwickTree<sum_abel<i64>> ft(N);
        test::NaiveArray<i64> naive(N, 0);

        i32 Q = rnd.next(MIN_Q, MAX_Q);
        while(Q--) {
            i32 p = rnd.next(N);
            i64 a = rnd.next(MIN_A, MAX_A);
            ft.add(p, a);
            naive.add(p, a);
        }

        for(i32 l = 0; l < N; ++l) for(i32 r = l + 1; r <= N; ++r) {
            ASSERT_EQ(ft.sum(l, r), naive.sum(l, r));
        }
    }
}