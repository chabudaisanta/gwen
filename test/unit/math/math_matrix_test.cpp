#include <gtest/gtest.h>

#include "gwen/math/matrix.hpp"

using namespace gwen;

TEST(MatrixTest, DoubleMatrix) {
    Matrix<double> a(2, 2);
    a[0][0] = 1.0;
    a[0][1] = 2.0;
    a[1][0] = 3.0;
    a[1][1] = 4.0;

    EXPECT_EQ(a.rows(), 2);
    EXPECT_EQ(a.cols(), 2);
    EXPECT_TRUE(a.is_square());

    Matrix<double> b(2, 2);
    b[0][0] = 2.0;
    b[0][1] = 0.0;
    b[1][0] = 1.0;
    b[1][1] = 2.0;

    auto c = a + b;
    EXPECT_EQ(c[0][0], 3.0);
    EXPECT_EQ(c[0][1], 2.0);
    EXPECT_EQ(c[1][0], 4.0);
    EXPECT_EQ(c[1][1], 6.0);

    auto d = a * b;  // [[1*2+2*1, 1*0+2*2], [3*2+4*1, 3*0+4*2]] = [[4, 4], [10, 8]]
    EXPECT_EQ(d[0][0], 4.0);
    EXPECT_EQ(d[0][1], 4.0);
    EXPECT_EQ(d[1][0], 10.0);
    EXPECT_EQ(d[1][1], 8.0);

    EXPECT_DOUBLE_EQ(a.det(), -2.0);  // 1*4 - 2*3 = -2
    EXPECT_EQ(a.rank(), 2);
    EXPECT_DOUBLE_EQ(a.trace(), 5.0);

    auto inv = a.inverse();
    // inverse of [[1, 2], [3, 4]] is [[-2, 1], [1.5, -0.5]]
    EXPECT_DOUBLE_EQ(inv[0][0], -2.0);
    EXPECT_DOUBLE_EQ(inv[0][1], 1.0);
    EXPECT_DOUBLE_EQ(inv[1][0], 1.5);
    EXPECT_DOUBLE_EQ(inv[1][1], -0.5);
}
