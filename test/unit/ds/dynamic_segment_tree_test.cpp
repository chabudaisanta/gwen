// clang-format off
#include "gwen/ds/dynamic_segment_tree.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"
#include <random>
#include <map>

using namespace gwen;

TEST(DynamicSegmentTreeTest, BasicOperations) {
    i64 n = 1000000000000LL;  // 10^12
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

    st.set(1000000, 50);  // overwrite
    EXPECT_EQ(st.get(1000000), 50);
    EXPECT_EQ(st.prod(0, n), 90);
}

TEST(DynamicSegmentTreeTest, RandomTest) {
    std::mt19937_64 rng(42);
    i64 n = 1000000000000LL;
    DynamicSegmentTree<sum_monoid<i64>> st(n);
    std::map<i64, i64> ref;
    
    for (int q = 0; q < 1000; ++q) {
        int type = rng() % 2;
        if (type == 0) {
            i64 p = rng() % n;
            i64 x = rng() % 1000;
            ref[p] = x;
            st.set(p, x);
        } else {
            i64 l = rng() % n;
            i64 r = rng() % (n + 1);
            if (l > r) std::swap(l, r);
            i64 expected = 0;
            auto it = ref.lower_bound(l);
            while (it != ref.end() && it->first < r) {
                expected += it->second;
                ++it;
            }
            EXPECT_EQ(st.prod(l, r), expected);
        }
    }
}

