#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include "gwen/container/mdarray.hpp"
#include "gwen/my_template.hpp"

using namespace gwen;

TEST(MdArrayTest, Dimensions2) {
    mdarray<int, 2> a({2, 3}, -1);
    EXPECT_EQ(a.shape[0], 2);
    EXPECT_EQ(a.shape[1], 3);
    EXPECT_EQ(a.size(), 6);
    a(0, 1) = 10;
    a(1, 2) = 20;
    EXPECT_EQ(a.raw_data()[1], 10);
    EXPECT_EQ(a.raw_data()[5], 20);
    EXPECT_EQ(a(0, 1), 10);
    EXPECT_EQ(a(1, 2), 20);
    EXPECT_EQ(a.view()(0, 1), 10);
}

TEST(MdArrayTest, Dimensions3) {
    mdarray<int, 3> a({2, 2, 2});
    for (i32 i = 0; i < 2; ++i)
        for (i32 j = 0; j < 2; ++j)
            for (i32 k = 0; k < 2; ++k) a(i, j, k) = i * 100 + j * 10 + k;
    EXPECT_EQ(a(1, 0, 1), 101);
    EXPECT_EQ(a.raw_data()[7], 111);
}

TEST(MdArrayTest, Dimensions1) {
    mdarray<int, 1> a(4);
    a(2) = 42;
    EXPECT_EQ(a(2), 42);
}

TEST(MdArrayTest, ConstAccess) {
    const mdarray<int, 2> a({2, 2}, 7);
    EXPECT_EQ(a(1, 0), 7);
}

TEST(MdArrayTest, StreamInput) {
    mdarray<int, 2> a(2, 3);
    std::stringstream ss("10 20 30 40 50 60");
    ss >> a;
    EXPECT_EQ(a(0, 0), 10);
    EXPECT_EQ(a(1, 2), 60);
}
