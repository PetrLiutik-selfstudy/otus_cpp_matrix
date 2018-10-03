#include <iostream>

#include "../inc/matrix.h"
#include "../inc/ver.h"

int main()
{
  std::cout << "matrix version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  using mat2d_t = mat::matrix<int, 0>;
  mat2d_t mat;

  // Заполнение матрицы.
  for(size_t i = 0; i < 10; ++i) {
      mat[i][i] = static_cast<int>(i);
      mat[i][9 - i] = static_cast<int>(i);
  }

  // Вывод значений.
  std::cout << "Matrix values: " << std::endl;
  for(size_t i = 1; i < 9; ++i) {
      for(size_t j = 1; j < 9; ++j)
          std::cout << mat[i][j] << " ";
      std::cout << std::endl;
  }

  // Итерирование по матрице.
  std::cout << "Matrix iterating: " << std::endl;
  for(auto it: mat) {
    size_t x;
    size_t y;
    int v;
    std::tie(x, y, v) = it;
    std::cout << x << " " << y << " " << v << std::endl;
  }

  // Размер матрицы.
  std::cout << "Matrix size: " << std::endl;
  std::cout << mat.size() << std::endl;

  // Оператор =.
  std::cout << "Canonical assignment: " << std::endl;
  ((mat[100][100] = 314) = 0) = 217;
  std::cout << mat[100][100] << std::endl;


  // 5-мерная матрица.
  using mat5d_t = mat::matrix<int, 0, 5>;
  mat5d_t mat1(mat5d_t::index_val_t(0, 0, 0, 0, 0, 777));
  mat5d_t mat2{mat5d_t::index_val_t(1, 2, 3, 4, 5, 100), mat5d_t::index_val_t(1, 2, 3, 4, 6, -100)};

  std::swap(mat1, mat2);

  mat5d_t mat3(std::forward<decltype(mat2)>(mat2));

  ((mat1[5][6][7][8][9] = 333) = 444) = 555;

  // Итерирование по матрице.
  std::cout << "Matrix5d iterating: " << std::endl;
  for(auto it: mat1) {
    size_t x1;
    size_t x2;
    size_t x3;
    size_t x4;
    size_t x5;
    int v;
    std::tie(x1, x2, x3, x4, x5, v) = it;
    std::cout << x1 << " " << x2 << " " << x3 << " " << x4 << " " << x5 << " " << v << std::endl;
  }

  // Размер матрицы.
  std::cout << "Matrix5d size: " << std::endl;
  std::cout << mat1.size() << std::endl;

  return 0;
}
