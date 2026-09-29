// clang-format off
#include "gwen/bbst/prod_sorted_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

#include <string>

#include "gwen/alge/monoid.hpp"

using namespace gwen;

struct concat_monoid {
    using S = std::string;
    static S op(const S& l, const S& r) { return l + r; }
    static S e() { return {}; }
};

TEST(ProdSortedTreapTest, BasicOperations) {
    ProdSortedTreap<int, sum_monoid<int>> t;
    EXPECT_TRUE(t.empty());

    t.insert(10, 100);
    t.insert(5, 50);
    t.insert(20, 200);

    EXPECT_EQ(t.size(), 3);
    EXPECT_TRUE(t.contains(5));
    EXPECT_TRUE(t.contains(10));
    EXPECT_TRUE(t.contains(20));

    // [5, 10, 20] -> [50, 100, 200]
    EXPECT_EQ(t.prod(5, 11), 150);
    EXPECT_EQ(t.prod(0, 100), 350);
    EXPECT_EQ(t.prod_by_index(0, 3), 350);
    EXPECT_EQ(t.prod_by_index(1, 3), 300);
    EXPECT_EQ(t.prod_by_index(2, 2), 0);
    EXPECT_EQ(t.all_prod(), 350);

    EXPECT_EQ(t.kth(0), 5);
    EXPECT_EQ(t.kth(2), 20);

    t.insert(10, 100);  // multiple keys
    EXPECT_EQ(t.count(10), 2);
    const auto& const_t = t;
    EXPECT_EQ(const_t.count(10), 2);
    EXPECT_EQ(const_t.kth(2), 10);
    EXPECT_EQ(t.prod(10, 11), 200);

    t.erase(10);  // erase one
    EXPECT_EQ(t.count(10), 1);
    EXPECT_EQ(t.prod(10, 11), 100);

    t.set(10, 99);  // erase all 10s and set 99
    EXPECT_EQ(t.count(10), 1);
    EXPECT_EQ(t.prod(10, 11), 99);

    t.erase_all(10);
    EXPECT_EQ(t.count(10), 0);
    EXPECT_EQ(t.all_prod(), 250);

    auto it_begin = t.begin();
    EXPECT_NE(it_begin, t.end());
    EXPECT_EQ(*it_begin, 5);

    auto lb20 = t.lower_bound(20);
    EXPECT_EQ(*lb20, 20);

    auto ub5 = t.upper_bound(5);
    EXPECT_EQ(*ub5, 20);

    auto it_end = t.end();
    --it_end;
    EXPECT_EQ(*it_end, 20);
}

TEST(ProdSortedTreapTest, EraseAllReleasesNodes) {
    using Treap = ProdSortedTreap<int, sum_monoid<int>>;
    const i32 initial_pool_size = Treap::d.size();
    Treap t;
    for (int i = 0; i < 100; ++i) t.insert(10, i);

    t.erase_all(10);

    EXPECT_TRUE(t.empty());
    EXPECT_EQ(Treap::d.size(), initial_pool_size);
}

TEST(ProdSortedTreapTest, ProductByIndexPreservesOrder) {
    ProdSortedTreap<int, concat_monoid> t;
    t.insert(1, "a");
    t.insert(3, "c");
    t.insert(2, "b");

    EXPECT_EQ(t.prod_by_index(0, 3), "abc");
    EXPECT_EQ(t.prod_by_index(1, 3), "bc");
}
