// clang-format off
#include "gwen/algo/idxsort.hpp"
#include "gwen/algo/runlength.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace gwen;

TEST(AlgorithmTest, IdxSort) {
    std::vector<int> v = {30, 10, 40, 20};
    auto idx = idxsort(v);
    EXPECT_EQ(idx.size(), 4);
    EXPECT_EQ(idx[0], 1);  // 10
    EXPECT_EQ(idx[1], 3);  // 20
    EXPECT_EQ(idx[2], 0);  // 30
    EXPECT_EQ(idx[3], 2);  // 40

    auto rev_idx = idxsort(v, true);
    EXPECT_EQ(rev_idx[0], 2);  // 40
    EXPECT_EQ(rev_idx[1], 0);  // 30
    EXPECT_EQ(rev_idx[2], 3);  // 20
    EXPECT_EQ(rev_idx[3], 1);  // 10
}

TEST(AlgorithmTest, RunLength) {
    std::string s = "aabbbca";
    auto rl = runlength(s.begin(), s.end());
    EXPECT_EQ(rl.size(), 4);
    EXPECT_EQ(rl[0], std::make_pair('a', 2));
    EXPECT_EQ(rl[1], std::make_pair('b', 3));
    EXPECT_EQ(rl[2], std::make_pair('c', 1));
    EXPECT_EQ(rl[3], std::make_pair('a', 1));

    std::vector<int> v;
    auto rl_empty = runlength(v.begin(), v.end());
    EXPECT_TRUE(rl_empty.empty());
}
