#include "gwen/math/xor_basis.hpp"
#include <gtest/gtest.h>

using namespace gwen;

TEST(XorBasisTest, BasicOperations) {
    XorBasis<u32> basis;
    EXPECT_TRUE(basis.empty());
    EXPECT_EQ(basis.size(), 0);

    // 1010
    EXPECT_TRUE(basis.insert(10));
    EXPECT_EQ(basis.size(), 1);
    EXPECT_FALSE(basis.empty());

    // 0110
    EXPECT_TRUE(basis.insert(6));
    EXPECT_EQ(basis.size(), 2);

    // 1100 = 1010 ^ 0110 (10 ^ 6 = 12)
    // Should not be inserted
    EXPECT_FALSE(basis.insert(12));
    EXPECT_EQ(basis.size(), 2);

    // Check can_form
    EXPECT_TRUE(basis.can_form(10));
    EXPECT_TRUE(basis.can_form(6));
    EXPECT_TRUE(basis.can_form(12));
    EXPECT_TRUE(basis.can_form(0));
    EXPECT_FALSE(basis.can_form(1));
    EXPECT_FALSE(basis.can_form(15));
}

TEST(XorBasisTest, Merge) {
    XorBasis<u32> b1, b2;
    b1.insert(10); // 1010
    b2.insert(6);  // 0110
    b2.insert(1);  // 0001

    auto b3 = XorBasis<u32>::op(b1, b2);
    EXPECT_EQ(b3.size(), 3);
    EXPECT_TRUE(b3.can_form(10));
    EXPECT_TRUE(b3.can_form(6));
    EXPECT_TRUE(b3.can_form(1));
    EXPECT_TRUE(b3.can_form(10 ^ 6 ^ 1)); // 15
    EXPECT_FALSE(b3.can_form(2)); // 0010 cannot be formed, because 10, 6, 1 forms {0000, 1010, 0110, 0001, 1100, 1011, 0111, 1101}
}
