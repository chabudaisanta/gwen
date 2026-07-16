// clang-format off
#include "gwen/ds/dynamic_segment_tree.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"

using namespace gwen;

TEST(DynamicSegmentTreeTest, BasicOperations) {
    i64 n = 1000000000000LL; // 10^12
    DynamicSegmentTree<sum_monoid<i64>> st(n);
    
    EXPECT_EQ(st.all_prod(), 0);
    
    st.set(100, 10);
    st.set(1000000, 20);
    st.set(500000000000LL, 30);
    
    EXPECT_EQ(st.get(100), 10);
    EXPECT_EQ(st.get(1000000), 20);
    EXPECT_EQ(st.get(500000000000LL), 30);
    EXPECT_EQ(st.get(50), 0);
    
    EXPECT_EQ(st.prod(0, 101), 10);
    EXPECT_EQ(st.prod(100, 1000001), 30);
    EXPECT_EQ(st.prod(0, n), 60);
    
    st.set(1000000, 50); // overwrite
    EXPECT_EQ(st.get(1000000), 50);
    EXPECT_EQ(st.prod(0, n), 90);
}
