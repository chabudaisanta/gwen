#include <gtest/gtest.h>

// clang-format off
#include "gwen/query/range_add_update_sum.hpp"
// clang-format on
#include "gwen/mod/modint.hpp"
#include "gwen/utils/xorshift.hpp"

using namespace gwen;
using mint = DynamicModInt64;

TEST(RangeAddUpdateSumTest, Simple) {
    mint::set_mod(998244353);
    std::vector<mint> a = {mint(1), mint(2), mint(3), mint(4), mint(5)};
    RangeAddUpdateSum<mint> seg(a);

    EXPECT_EQ(seg.prod(0, 5), mint(15));
    
    // add 1 to [1, 4) -> {1, 3, 4, 5, 5}
    seg.add(1, 4, mint(1));
    EXPECT_EQ(seg.get(0), mint(1));
    EXPECT_EQ(seg.get(1), mint(3));
    EXPECT_EQ(seg.get(2), mint(4));
    EXPECT_EQ(seg.get(3), mint(5));
    EXPECT_EQ(seg.get(4), mint(5));
    EXPECT_EQ(seg.prod(0, 5), mint(18));

    // update 10 to [2, 5) -> {1, 3, 10, 10, 10}
    seg.update(2, 5, mint(10));
    EXPECT_EQ(seg.prod(0, 5), mint(34));

    // multiply 2 to [0, 3) -> {2, 6, 20, 10, 10}
    seg.multiply(0, 3, mint(2));
    EXPECT_EQ(seg.prod(0, 3), mint(28));
    EXPECT_EQ(seg.prod(3, 5), mint(20));

    // affine 2x + 1 to [1, 4) -> {2, 13, 41, 21, 10}
    seg.affine(1, 4, mint(2), mint(1));
    EXPECT_EQ(seg.get(0), mint(2));
    EXPECT_EQ(seg.get(1), mint(13));
    EXPECT_EQ(seg.get(2), mint(41));
    EXPECT_EQ(seg.get(3), mint(21));
    EXPECT_EQ(seg.get(4), mint(10));
    EXPECT_EQ(seg.prod(0, 5), mint(87));
}
