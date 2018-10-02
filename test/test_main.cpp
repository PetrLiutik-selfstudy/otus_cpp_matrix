#include "gtest/gtest.h"
#include "../inc/matrix.h"
#include "../inc/ver.h"

TEST(ver_test_case, ver_major_test) {
  EXPECT_GE(ver_major(), 1);
}

TEST(ver_test_case, ver_minor_test) {
  EXPECT_GE(ver_minor(), 1);
}

TEST(ver_test_case, ver_patch_test) {
  EXPECT_GE(ver_patch(), 1);
}

TEST(matrix_test_case, matrix_default_ctor_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat;
  EXPECT_EQ(mat.size(), 0);
}

TEST(matrix_test_case, matrix_at_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat;

  mat[5][101] = 100;
  EXPECT_EQ(mat.size(), 1);
  EXPECT_EQ(mat[5][101], 100);

  mat[5][101] = 0;
  EXPECT_EQ(mat.size(), 0);
  EXPECT_EQ(mat[5][101], 0);
}

TEST(matrix_test_case, matrix_copy_ctor_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat1;
  mat1[12][67] = 100;

  mat_t mat2(mat1);
  EXPECT_EQ(mat2.size(), 1);
  EXPECT_EQ(mat2, mat1);
}

TEST(matrix_test_case, matrix_move_ctor_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat1;
  mat1[92][1] = 100;
  mat_t mat2(std::forward<decltype(mat1)>(mat1));
  
  EXPECT_EQ(mat2.size(), 1);
  EXPECT_EQ(mat2[92][1], 100);
  EXPECT_EQ(mat1.size(), 0);
  EXPECT_EQ(mat1[92][1], 0);
}

TEST(matrix_test_case, matrix_swap_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat1;
  mat1[92][1] = 100;
  mat_t mat2;
  mat2[1][92] = -100;
  std::swap(mat1, mat2);

  EXPECT_EQ(mat2.size(), 1);
  EXPECT_EQ(mat2[92][1], 100);
  EXPECT_EQ(mat1.size(), 1);
  EXPECT_EQ(mat1[1][92], -100);
}

TEST(matrix_test_case, matrix_empty_val_test) {
  using mat0_t = mat::matrix<int, 0>;
  mat0_t mat0;
  EXPECT_EQ(mat0[75][31], 0);

  mat0[75][31] = 0;
  EXPECT_EQ(mat0.size(), 0);
  EXPECT_EQ(mat0[75][31], 0);

  using mat1_t = mat::matrix<int, 1>;
  mat1_t mat1;
  EXPECT_EQ(mat1[75][31], 1);

  mat1[75][31] = 1;
  EXPECT_EQ(mat1.size(), 0);
  EXPECT_EQ(mat1[75][31], 1);
}

TEST(matrix_test_case, matrix_iter_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat;
  EXPECT_EQ(mat.begin(), mat.end());

  mat[5][75] = 3;
  mat[98][7] = 7;
  mat[8][5]  = 10;
  mat[25][3] = 100;
  mat[6][11] = -100;

  size_t size = 0;
  for(auto it: mat) {
    size_t x;
    size_t y;
    int v;
    std::tie(x, y, v) = it;
    EXPECT_EQ(mat[x][y], v);
    size++;
  }
  EXPECT_EQ(mat.size(), size);

  size = 0;
  for(auto it = mat.begin(); it != mat.end(); ++it) {
    size_t x;
    size_t y;
    int v;
    std::tie(x, y, v) = *it;
    EXPECT_EQ(mat[x][y], v);
    size++;
  }
  EXPECT_EQ(mat.size(), size);
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
