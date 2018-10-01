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

TEST(matrix_test_case, matrix_ctor_test) {
  using mat2d_t = mat::matrix<int, 0>;
  mat2d_t mat;
  EXPECT_EQ(mat.size(), 0);
  
  mat[0][0] = 100;
  EXPECT_EQ(mat.size(), 1);
  EXPECT_EQ(mat[0][0], 100);
  
  mat[0][0] = 0;
  EXPECT_EQ(mat.size(), 0);
  EXPECT_EQ(mat[0][0], 0);
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
