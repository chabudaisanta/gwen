// clang-format off
#include "gwen/bbst/prod_implicit_treap.hpp"
// clang-format on

#include <gtest/gtest.h>

#include "gwen/alge/monoid.hpp"

using namespace gwen;

TEST(ProdImplicitTreapTest, SumMonoid) {
    ProdImplicitTreap<sum_monoid<int>> t;
    t.push_back(1);
    t.push_back(2);
    t.push_back(3);
    t.push_back(4);

    // [1, 2, 3, 4]
    EXPECT_EQ(t.prod(0, 4), 10);
    EXPECT_EQ(t.prod(1, 3), 5);  // 2 + 3

    t.reverse(1, 4);
    // [1, 4, 3, 2]
    EXPECT_EQ(t.prod(1, 3), 7);  // 4 + 3
    EXPECT_EQ(t.get(1), 4);

    const auto& const_t = t;
    EXPECT_EQ(const_t.all_prod(), 10);
}

#ifndef NDEBUG
TEST(ProdImplicitTreapTest, RejectsSelfConcat) {
    ProdImplicitTreap<sum_monoid<int>> t({1, 2, 3});
    EXPECT_DEATH(t.concat(t), "");
    EXPECT_DEATH(ProdImplicitTreap<sum_monoid<int>>::concat(t, t), "");
}
#else
TEST(ProdImplicitTreapTest, SelfConcatIsSafeWithoutAssertions) {
    ProdImplicitTreap<sum_monoid<int>> t({1, 2, 3});
    t.concat(t);
    EXPECT_EQ(t.to_vec(), (std::vector<int>{1, 2, 3}));

    auto result = ProdImplicitTreap<sum_monoid<int>>::concat(t, t);
    EXPECT_TRUE(result.empty());
    EXPECT_EQ(t.to_vec(), (std::vector<int>{1, 2, 3}));
}
#endif
