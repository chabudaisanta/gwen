// clang-format off
#include "gwen/ds/segment_tree.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"

using namespace gwen;

TEST(SegmentTreeTest, BasicOperations) {
    SegmentTree<sum_monoid<int>> st(10);
    for (int i = 0; i < 10; ++i) {
        st.set(i, i + 1);
    }

    EXPECT_EQ(st.prod(0, 10), 55);
    EXPECT_EQ(st.prod(2, 5), 3 + 4 + 5);
    EXPECT_EQ(st.all_prod(), 55);

    st.set(2, 10);
    EXPECT_EQ(st.get(2), 10);
    EXPECT_EQ(st.prod(2, 5), 10 + 4 + 5);

    auto f = [](int x) { return x < 15; };
    // l = 2, max_right gives first idx r where prod(l, r) >= 15 -> not < 15
    // prod(2, 2) = 0 < 15
    // prod(2, 3) = 10 < 15
    // prod(2, 4) = 14 < 15
    // prod(2, 5) = 19 >= 15
    EXPECT_EQ(st.max_right(2, f), 4);

    // r = 5, min_left gives first idx l where prod(l, r) >= 15 -> not < 15
    // prod(5, 5) = 0 < 15
    // prod(4, 5) = 5 < 15
    // prod(3, 5) = 9 < 15
    // prod(2, 5) = 19 >= 15
    EXPECT_EQ(st.min_left(5, f), 3);
}

TEST(SegmentTreeTest, VectorConstructor) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    SegmentTree<sum_monoid<int>> st(v);

    EXPECT_EQ(st.prod(0, 5), 15);
    EXPECT_EQ(st.prod(1, 4), 9);
}

TEST(SegmentTreeTest, EdgeCases) {
    SegmentTree<sum_monoid<int>> st(0);
    EXPECT_EQ(st.all_prod(), 0);

    SegmentTree<sum_monoid<int>> st2(1);
    st2.set(0, 5);
    EXPECT_EQ(st2.prod(0, 1), 5);
    EXPECT_EQ(st2.prod(0, 0), 0);
    EXPECT_EQ(st2.prod(1, 1), 0);
}
