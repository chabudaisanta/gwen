// clang-format off
#include "gwen/math/stern_brocot_tree.hpp"
// clang-format on

#include <gtest/gtest.h>

using namespace gwen;

using Tree = SternBrocotTree<i64>;

TEST(SternBrocotTreeTest, EncodesAndDecodesPaths) {
    EXPECT_TRUE(Tree::encode_path(1, 1).empty());
    EXPECT_EQ(Tree::encode_path(1, 3), (std::vector<std::pair<char, i64>>{{'L', 2}}));
    EXPECT_EQ(Tree::encode_path(5, 3), (std::vector<std::pair<char, i64>>{{'R', 1}, {'L', 1}, {'R', 1}}));

    const std::vector<std::pair<char, i64>> path{{'R', 1}, {'L', 1}, {'R', 1}};
    EXPECT_EQ(Tree::decode_path(path), (std::pair<i64, i64>{5, 3}));
}

TEST(SternBrocotTreeTest, FindsLca) {
    EXPECT_EQ(Tree::lca(5, 3, 5, 3), (std::pair<i64, i64>{5, 3}));
    EXPECT_EQ(Tree::lca(5, 3, 2, 1), (std::pair<i64, i64>{2, 1}));
    EXPECT_EQ(Tree::lca(1, 5, 1, 3), (std::pair<i64, i64>{1, 3}));
    EXPECT_EQ(Tree::lca(5, 3, 3, 5), (std::pair<i64, i64>{1, 1}));
}

TEST(SternBrocotTreeTest, FindsAncestors) {
    EXPECT_EQ(Tree::ancestor(0, 5, 3), (std::pair<i64, i64>{1, 1}));
    EXPECT_EQ(Tree::ancestor(1, 5, 3), (std::pair<i64, i64>{2, 1}));
    EXPECT_EQ(Tree::ancestor(3, 5, 3), (std::pair<i64, i64>{5, 3}));
    EXPECT_EQ(Tree::ancestor(4, 5, 3), (std::pair<i64, i64>{0, 0}));
#ifndef NDEBUG
    EXPECT_DEATH((void)Tree::ancestor(-1, 5, 3), "");
#endif
}

TEST(SternBrocotTreeTest, ReturnsRange) {
    EXPECT_EQ(Tree::range(1, 1), (std::tuple<i64, i64, i64, i64>{0, 1, 1, 0}));
    EXPECT_EQ(Tree::range(1, 3), (std::tuple<i64, i64, i64, i64>{0, 1, 1, 2}));
    EXPECT_EQ(Tree::range(5, 3), (std::tuple<i64, i64, i64, i64>{3, 2, 2, 1}));
}
