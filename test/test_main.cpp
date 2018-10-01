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

  mat[0][0] = 100;
  EXPECT_EQ(mat.size(), 1);
  EXPECT_EQ(mat[0][0], 100);

  mat[0][0] = 0;
  EXPECT_EQ(mat.size(), 0);
  EXPECT_EQ(mat[0][0], 0);
}

TEST(matrix_test_case, matrix_copy_ctor_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat1;
  mat1[0][0] = 100;

  mat_t mat2(mat1);
  EXPECT_EQ(mat2.size(), 1);
  EXPECT_EQ(mat2, mat1);
}

TEST(matrix_test_case, matrix_move_ctor_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat1;
  mat1[0][0] = 100;
  mat_t mat2(std::forward<decltype(mat1)>(mat1));
  
  EXPECT_EQ(mat2.size(), 1);
  EXPECT_EQ(mat2[0][0], 100);
  EXPECT_EQ(mat1.size(), 0);
  EXPECT_EQ(mat1[0][0], 0);
}

TEST(matrix_test_case, matrix_empty_val_test) {
  using mat_t = mat::matrix<int, 0>;
  mat_t mat;
  EXPECT_EQ(mat[0][0], 0);

  mat[0][0] = 0;
  EXPECT_EQ(mat.size(), 0);
  EXPECT_EQ(mat[0][0], 0);
  
  using mat_1_t = mat::matrix<int, 1>;
  mat_1_t mat1;
  EXPECT_EQ(mat1[0][0], 1);

  mat1[0][0] = 1;
  EXPECT_EQ(mat.size(), 0);
  EXPECT_EQ(mat[0][0], 1);
}




int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
