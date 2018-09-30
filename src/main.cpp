#include <iostream>

#include "../inc/matrix.h"
#include "../inc/ver.h"


int main()
{
  std::cout << "matrix version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;


  using mat2_t = matrix<int, 0>;
  mat2_t mat;

  using mat3_t = matrix<int, 0, 3>;
  mat3_t mat1;

////  auto index = std::make_tuple<int, int>(1, 1);

//  mat.set(ind, val);

  mat1[1][2][3] = 5;
  mat1[100][500][0] = 1200;

  using tuple3_t = mat3_t::index_val_t;
  mat3_t mat2{tuple3_t(1, 2, 3, 4), tuple3_t(4, 3, 2, 1)};

////  std::cout << mat.get(ind) << std::endl;
//  std::cout << mat[1][2][3][4][5] << std::endl;


  for(auto i = 0; i < 10; ++i) {
      mat[i][i] = i;
      mat[i][9 - i] = i;
  }

  ((mat[0][0] = 314) = 0) = 9;

  for(auto i = 0; i < 10; ++i) {
      for(auto j = 0; j < 10; ++j)
          std::cout << mat[i][j] << " ";
      std::cout << std::endl;
  }

  for(auto it: mat1) {
    int x;
    int y;
    int z;
    int v;
    std::tie(x, y, z, v) = it;
    std::cout << x << y << z << v << std::endl;
  }

  for(auto it: mat2) {
    int x;
    int y;
    int z;
    int v;
    std::tie(x, y, z, v) = it;
    std::cout << x << y << z << v << std::endl;
  }

  std::cout << mat.size() << std::endl;

  return 0;
}
