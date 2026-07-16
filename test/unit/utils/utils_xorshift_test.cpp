// clang-format off
#include "gwen/utils/xorshift.hpp"
// clang-format on

#include <gtest/gtest.h>
#include <unordered_set>

using namespace gwen;

TEST(XorShiftTest, Randomness) {
    std::unordered_set<u64> s;
    for (int i = 0; i < 1000; ++i) {
        s.insert(rand64());
    }
    // High probability they are all distinct
    EXPECT_GE(s.size(), 990);
}

TEST(XorShiftTest, Range32) {
    for (int i = 0; i < 1000; ++i) {
        u32 v = rand32(10);
        EXPECT_GE(v, 0);
        EXPECT_LT(v, 10);
        
        u32 v2 = rand32(5, 15);
        EXPECT_GE(v2, 5);
        EXPECT_LT(v2, 15);
    }
}
