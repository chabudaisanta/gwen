// clang-format off
#include "gwen/template.hpp"
// clang-format on

#include <gtest/gtest.h>

TEST(TemplateTest, CompileTest) {
    // Just a compilation test to ensure all includes and macros are working
    std::vector<int> v = {3, 1, 4};
    EXPECT_EQ(SUM(v), 8);
    EXPECT_TRUE(chmax(v[0], 5));
    EXPECT_EQ(v[0], 5);
}
