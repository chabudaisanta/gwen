#include <gtest/gtest.h>

// clang-format off
#include "gwen/ds/wavelet_matrix.hpp"
#include "gwen/ds/bit_vector.hpp"
// clang-format on

using namespace gwen;

TEST(BitVectorTest, BasicOperations) {
    BitVector bv(15);
    bv.set(1);
    bv.set(2);
    bv.set(5);
    bv.set(10);
    bv.build();

    EXPECT_EQ(bv.get(1), true);
    EXPECT_EQ(bv.get(3), false);

    EXPECT_EQ(bv.rank1(0), 0);
    EXPECT_EQ(bv.rank1(2), 1); // [0, 1] has one 1
    EXPECT_EQ(bv.rank1(6), 3); // 1, 2, 5
    EXPECT_EQ(bv.rank1(15), 4); // 1, 2, 5, 10

    EXPECT_EQ(bv.rank0(2), 1); // 0
    EXPECT_EQ(bv.rank0(6), 3); // 0, 3, 4
    EXPECT_EQ(bv.rank0(15), 11);

    EXPECT_EQ(bv.select1(0), 1);
    EXPECT_EQ(bv.select1(1), 2);
    EXPECT_EQ(bv.select1(2), 5);
    EXPECT_EQ(bv.select1(3), 10);
    EXPECT_EQ(bv.select1(4), 15);

    EXPECT_EQ(bv.select0(0), 0);
    EXPECT_EQ(bv.select0(1), 3);
    EXPECT_EQ(bv.select0(2), 4);
}

TEST(WaveletMatrixTest, BasicOperations) {
    std::vector<int> a = {5, 4, 5, 5, 2, 1, 5, 6, 1, 3, 5, 0};
    WaveletMatrix<int> wm(a);

    EXPECT_EQ(wm.access(0), 5);
    EXPECT_EQ(wm.access(4), 2);
    EXPECT_EQ(wm.access(11), 0);

    // kth_smallest
    EXPECT_EQ(wm.kth_smallest(0, 12, 0), 0);
    EXPECT_EQ(wm.kth_smallest(0, 12, 11), 6);
    EXPECT_EQ(wm.kth_smallest(0, 5, 0), 2);
    EXPECT_EQ(wm.kth_smallest(0, 5, 4), 5);

    // kth_largest
    EXPECT_EQ(wm.kth_largest(0, 12, 0), 6);
    EXPECT_EQ(wm.kth_largest(0, 12, 11), 0);

    // range_freq
    EXPECT_EQ(wm.range_freq(0, 12, 5), 6);
    EXPECT_EQ(wm.range_freq(0, 12, 6), 11);
    EXPECT_EQ(wm.range_freq(0, 12, 1, 5), 5);

    // prev_value / next_value
    EXPECT_EQ(wm.prev_value(0, 12, 5).value_or(-1), 4);
    EXPECT_EQ(wm.next_value(0, 12, 4).value_or(-1), 4);
    EXPECT_EQ(wm.next_value(0, 12, 5).value_or(-1), 5);
    EXPECT_EQ(wm.next_value(0, 12, 7).value_or(-1), -1);
}
