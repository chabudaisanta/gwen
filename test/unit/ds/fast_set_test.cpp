#include "gwen/ds/fast_set.hpp"
#include <gtest/gtest.h>
#include <set>
#include <random>

using namespace gwen;

TEST(FastSetTest, Basic) {
    FastSet st(100);
    EXPECT_FALSE(st.contains(10));
    st.insert(10);
    EXPECT_TRUE(st.contains(10));
    EXPECT_FALSE(st.contains(11));
    
    st.insert(20);
    EXPECT_EQ(st.next(0), 10);
    EXPECT_EQ(st.next(10), 10);
    EXPECT_EQ(st.next(11), 20);
    EXPECT_EQ(st.next(20), 20);
    EXPECT_EQ(st.next(21), 100);

    EXPECT_EQ(st.prev(100), 20);
    EXPECT_EQ(st.prev(21), 20);
    EXPECT_EQ(st.prev(20), 20);
    EXPECT_EQ(st.prev(19), 10);
    EXPECT_EQ(st.prev(10), 10);
    EXPECT_EQ(st.prev(9), -1);

    st.erase(10);
    EXPECT_FALSE(st.contains(10));
    EXPECT_EQ(st.next(0), 20);
    EXPECT_EQ(st.prev(19), -1);
}

TEST(FastSetTest, Boundary) {
    FastSet st(64);
    st.insert(0);
    st.insert(63);
    EXPECT_TRUE(st.contains(0));
    EXPECT_TRUE(st.contains(63));

    EXPECT_EQ(st.next(1), 63);
    EXPECT_EQ(st.prev(62), 0);

    st.erase(0);
    st.erase(63);
    EXPECT_EQ(st.next(0), 64);
    EXPECT_EQ(st.prev(63), -1);
}

TEST(FastSetTest, Empty) {
    FastSet st(0);
    EXPECT_FALSE(st.contains(0));
    st.insert(0);
    EXPECT_FALSE(st.contains(0));
    EXPECT_EQ(st.next(0), 0);
    EXPECT_EQ(st.prev(0), -1);

    FastSet st2(100);
    EXPECT_EQ(st2.next(0), 100);
    EXPECT_EQ(st2.prev(99), -1);
}

TEST(FastSetTest, LargeSize) {
    i32 n = 1000000;
    FastSet st(n);
    st.insert(n - 1);
    st.insert(0);
    st.insert(500000);

    EXPECT_EQ(st.next(1), 500000);
    EXPECT_EQ(st.next(500001), n - 1);
    EXPECT_EQ(st.next(n), n);

    EXPECT_EQ(st.prev(n - 2), 500000);
    EXPECT_EQ(st.prev(499999), 0);
    EXPECT_EQ(st.prev(-1), -1);
}

TEST(FastSetTest, RandomSetComparison) {
    i32 n = 1000;
    FastSet st(n);
    std::set<i32> ref;

    std::mt19937 mt(1337);
    std::uniform_int_distribution<i32> dist_op(0, 3);
    std::uniform_int_distribution<i32> dist_val(0, n - 1);

    for (int i = 0; i < 10000; ++i) {
        i32 op = dist_op(mt);
        i32 x = dist_val(mt);

        if (op == 0) { // insert
            st.insert(x);
            ref.insert(x);
        } else if (op == 1) { // erase
            st.erase(x);
            ref.erase(x);
        } else if (op == 2) { // next
            i32 act = st.next(x);
            auto it = ref.lower_bound(x);
            i32 exp = (it == ref.end()) ? n : *it;
            EXPECT_EQ(act, exp);
        } else { // prev
            i32 act = st.prev(x);
            auto it = ref.upper_bound(x);
            i32 exp = -1;
            if (it != ref.begin()) {
                --it;
                exp = *it;
            }
            EXPECT_EQ(act, exp);
        }
        
        EXPECT_EQ(st.contains(x), ref.count(x) > 0);
    }
}
