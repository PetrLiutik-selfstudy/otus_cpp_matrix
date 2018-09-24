#include <iostream>

#include "../inc/ver.h"

template<typename T, T default_val, size_t N>
class matrix {



};

int main()
{
  std::cout << "matrix version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;







  return 0;
}
