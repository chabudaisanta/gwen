// clang-format off
#include "gwen/utils/compress.hpp"
// clang-format on

#include <gtest/gtest.h>
#include <vector>

using namespace gwen;

TEST(CompressTest, Basic) {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::vector<i32> res = compress(v);
    
    // sorted unique: 1, 2, 3, 4, 5, 6, 9
    // ranks:
    // 1 -> 0
    // 2 -> 1
    // 3 -> 2
    // 4 -> 3
    // 5 -> 4
    // 6 -> 5
    // 9 -> 6
    
    std::vector<i32> expected = {2, 0, 3, 0, 4, 6, 1, 5, 4, 2, 4};
    EXPECT_EQ(res, expected);
}
