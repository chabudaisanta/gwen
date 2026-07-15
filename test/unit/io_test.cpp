// clang-format off
#include "gwen/io/vector.hpp"
#include "gwen/io/yesno.hpp"
// clang-format on

#include <gtest/gtest.h>
#include <sstream>
#include <vector>

using namespace gwen;

TEST(IoTest, VectorOutput) {
    std::vector<int> v = {1, 2, 3};
    std::stringstream ss;
    ss << v;
    EXPECT_EQ(ss.str(), "1 2 3");

    std::vector<int> empty_v;
    std::stringstream ss2;
    ss2 << empty_v;
    EXPECT_EQ(ss2.str(), "");
}

TEST(IoTest, VectorInput) {
    std::vector<int> v(3);
    std::stringstream ss("4 5 6");
    ss >> v;
    EXPECT_EQ(v[0], 4);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(v[2], 6);
}

TEST(IoTest, YesNoOutput) {
    std::stringstream ss;
    auto old_buf = std::cout.rdbuf(ss.rdbuf());
    yon(true);
    yon(false);
    std::cout.rdbuf(old_buf);
    EXPECT_EQ(ss.str(), "Yes\nNo\n");
}
