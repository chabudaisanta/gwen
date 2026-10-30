// clang-format off
#include "gwen/ds/sparse_table.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <random>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

using namespace gwen;

TEST(SparseTableTest, BasicAndEmptyRanges) {
    const std::vector<i32> values = {7, 2, 5, 1, 8, 3, 6};
    SparseTable<min_monoid<i32>> table(values);

    EXPECT_EQ(table.size(), i32(values.size()));
    EXPECT_EQ(table.prod(0, 7), 1);
    EXPECT_EQ(table.prod(2, 6), 1);
    EXPECT_EQ(table.prod(3, 4), 1);
    EXPECT_EQ(table.prod(4, 7), 3);
    EXPECT_EQ(table.prod(2, 2), std::numeric_limits<i32>::max());

    SparseTable<min_monoid<i32>> empty({});
    EXPECT_EQ(empty.size(), 0);
    EXPECT_EQ(empty.prod(0, 0), std::numeric_limits<i32>::max());
}

TEST(SparseTableTest, RandomRanges) {
    std::mt19937 rng(42);
    for (i32 n = 0; n <= 80; ++n) {
        std::vector<i32> values(usize(n), 0);
        for (auto& value : values) value = i32(rng() % 1000);
        SparseTable<min_monoid<i32>> table(values);

        for (i32 l = 0; l <= n; ++l) {
            for (i32 r = l; r <= n; ++r) {
                i32 expected = std::numeric_limits<i32>::max();
                for (i32 i = l; i < r; ++i) expected = std::min(expected, values[usize(i)]);
                EXPECT_EQ(table.prod(l, r), expected) << "n=" << n << ", l=" << l << ", r=" << r;
            }
        }
    }
}
