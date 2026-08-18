#include "gwen/ds/transformable_matrix.hpp"

#include <gtest/gtest.h>

#include <vector>

using namespace gwen;

TEST(TransformableMatrixTest, BasicAccess) {
    std::vector<std::vector<i32>> a = {{1, 2, 3}, {4, 5, 6}};
    TransformableMatrix<i32> mat(a);

    EXPECT_EQ(mat.h(), 2);
    EXPECT_EQ(mat.w(), 3);
    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 1), 2);
    EXPECT_EQ(mat(0, 2), 3);
    EXPECT_EQ(mat(1, 0), 4);
    EXPECT_EQ(mat(1, 1), 5);
    EXPECT_EQ(mat(1, 2), 6);

    mat(1, 1) = 10;
    EXPECT_EQ(mat(1, 1), 10);
}

TEST(TransformableMatrixTest, SwapRowsAndColumns) {
    std::vector<std::vector<i32>> a = {{1, 2, 3}, {4, 5, 6}};
    TransformableMatrix<i32> mat(a);

    mat.swap_rows(0, 1);
    EXPECT_EQ(mat.h(), 2);
    EXPECT_EQ(mat(0, 0), 4);
    EXPECT_EQ(mat(0, 1), 5);
    EXPECT_EQ(mat(0, 2), 6);
    EXPECT_EQ(mat(1, 0), 1);
    EXPECT_EQ(mat(1, 1), 2);
    EXPECT_EQ(mat(1, 2), 3);

    mat.swap_columns(0, 2);
    EXPECT_EQ(mat.w(), 3);
    EXPECT_EQ(mat(0, 0), 6);
    EXPECT_EQ(mat(0, 1), 5);
    EXPECT_EQ(mat(0, 2), 4);
    EXPECT_EQ(mat(1, 0), 3);
    EXPECT_EQ(mat(1, 1), 2);
    EXPECT_EQ(mat(1, 2), 1);
}

TEST(TransformableMatrixTest, Transpose) {
    std::vector<std::vector<i32>> a = {{1, 2, 3}, {4, 5, 6}};
    TransformableMatrix<i32> mat(a);

    mat.transpose();
    EXPECT_EQ(mat.h(), 3);
    EXPECT_EQ(mat.w(), 2);

    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 1), 4);
    EXPECT_EQ(mat(1, 0), 2);
    EXPECT_EQ(mat(1, 1), 5);
    EXPECT_EQ(mat(2, 0), 3);
    EXPECT_EQ(mat(2, 1), 6);

    // modification should still point to correct data
    mat(1, 1) = 100;
    mat.transpose();
    EXPECT_EQ(mat(1, 1), 100);
}

TEST(TransformableMatrixTest, Rotate) {
    std::vector<std::vector<i32>> a = {{1, 2, 3}, {4, 5, 6}};
    TransformableMatrix<i32> mat(a);

    // Rotate 90 degrees clockwise
    mat.rotate(1);
    EXPECT_EQ(mat.h(), 3);
    EXPECT_EQ(mat.w(), 2);

    EXPECT_EQ(mat(0, 0), 4);
    EXPECT_EQ(mat(0, 1), 1);
    EXPECT_EQ(mat(1, 0), 5);
    EXPECT_EQ(mat(1, 1), 2);
    EXPECT_EQ(mat(2, 0), 6);
    EXPECT_EQ(mat(2, 1), 3);

    // Rotate 180 degrees clockwise from original
    mat.rotate(1);
    EXPECT_EQ(mat.h(), 2);
    EXPECT_EQ(mat.w(), 3);
    EXPECT_EQ(mat(0, 0), 6);
    EXPECT_EQ(mat(0, 1), 5);
    EXPECT_EQ(mat(0, 2), 4);
    EXPECT_EQ(mat(1, 0), 3);
    EXPECT_EQ(mat(1, 1), 2);
    EXPECT_EQ(mat(1, 2), 1);

    // Rotate back to original
    mat.rotate(2);
    EXPECT_EQ(mat.h(), 2);
    EXPECT_EQ(mat.w(), 3);
    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 1), 2);
    EXPECT_EQ(mat(0, 2), 3);

    // Negative rotation (counter-clockwise)
    mat.rotate(-1);
    EXPECT_EQ(mat.h(), 3);
    EXPECT_EQ(mat.w(), 2);
    EXPECT_EQ(mat(0, 0), 3);
    EXPECT_EQ(mat(0, 1), 6);
    EXPECT_EQ(mat(1, 0), 2);
    EXPECT_EQ(mat(1, 1), 5);
    EXPECT_EQ(mat(2, 0), 1);
    EXPECT_EQ(mat(2, 1), 4);
}

TEST(TransformableMatrixTest, ToVec) {
    std::vector<std::vector<i32>> a = {{1, 2}, {3, 4}};
    TransformableMatrix<i32> mat(a);
    mat.rotate(1);
    auto vec = mat.to_vec();

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0].size(), 2);
    EXPECT_EQ(vec[0][0], 3);
    EXPECT_EQ(vec[0][1], 1);
    EXPECT_EQ(vec[1][0], 4);
    EXPECT_EQ(vec[1][1], 2);
}
