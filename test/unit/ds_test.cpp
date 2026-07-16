// clang-format off
#include "gwen/ds/prefix_sum.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen;

TEST(DsTest, PrefixSum) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto ps = prefix_sum(v);
    EXPECT_EQ(ps.size(), 6);
    EXPECT_EQ(ps[0], 0);
    EXPECT_EQ(ps[1], 1);
    EXPECT_EQ(ps[3], 6);
    EXPECT_EQ(ps[5], 15);
}

TEST(DsTest, PrefixSum2D) {
    std::vector<std::vector<int>> v = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    PrefixSum2d<int> ps2(v);
    
    // get(l, r, u, d) [u, d) x [l, r)
    // l=0, r=3, u=0, d=3 -> 全体 45
    EXPECT_EQ(ps2.get(0, 3, 0, 3), 45);

    // l=1, r=3, u=1, d=3 -> {5,6}, {8,9} -> 28
    EXPECT_EQ(ps2.get(1, 3, 1, 3), 28);

    // l=0, r=1, u=0, d=1 -> {1} -> 1
    EXPECT_EQ(ps2.get(0, 1, 0, 1), 1);
}
