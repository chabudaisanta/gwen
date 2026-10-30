// clang-format off
#include "gwen/ds/disjoint_sparse_table.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <random>
#include <string>
#include <vector>

#include "gwen/alge/monoid.hpp"
#include "gwen/types.hpp"

using namespace gwen;

struct string_concat_monoid {
    using S = std::string;
    static S op(const S& a, const S& b) { return a + b; }
    static S e() { return {}; }
};

TEST(DisjointSparseTableTest, NonCommutativeOrderAndEmptyRanges) {
    const std::vector<std::string> values = {"a", "bc", "d", "ef", "g", "hi", "j"};
    DisjointSparseTable<string_concat_monoid> table(values);

    EXPECT_EQ(table.size(), i32(values.size()));
    EXPECT_EQ(table.prod(0, 7), "abcdefghij");
    EXPECT_EQ(table.prod(1, 6), "bcdefghi");
    EXPECT_EQ(table.prod(3, 4), "ef");
    EXPECT_EQ(table.prod(5, 5), "");

    DisjointSparseTable<string_concat_monoid> empty({});
    EXPECT_EQ(empty.size(), 0);
    EXPECT_EQ(empty.prod(0, 0), "");
}

TEST(DisjointSparseTableTest, RandomRanges) {
    std::mt19937 rng(42);
    for (i32 n = 0; n <= 80; ++n) {
        std::vector<i64> values(usize(n), 0);
        for (auto& value : values) value = i64(rng() % 1000);
        DisjointSparseTable<sum_monoid<i64>> table(values);

        for (i32 l = 0; l <= n; ++l) {
            for (i32 r = l; r <= n; ++r) {
                i64 expected = 0;
                for (i32 i = l; i < r; ++i) expected += values[usize(i)];
                EXPECT_EQ(table.prod(l, r), expected) << "n=" << n << ", l=" << l << ", r=" << r;
            }
        }
    }
}
