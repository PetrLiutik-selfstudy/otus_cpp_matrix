#include <iostream>

#include "../inc/ver.h"


#include <unordered_map>
#include <tuple>
#include <array>
#include <memory>

template <size_t ind, typename T, typename... Tail>
struct gen_tuple_t {
  using type = typename gen_tuple_t<ind - 1, T, T, Tail...>::type;
};

template <typename T, typename... Tail>
struct gen_tuple_t<0, T, Tail...> {
  using type = std::tuple<Tail...>;
};

template<typename T, T default_val, size_t N>
class matrix {
  public:
    using value_t = T;
    using index_elem_t = size_t;
    using index_t = gen_tuple_t<N, index_elem_t>;

    using ind_t = std::array<index_elem_t, N>;


    struct hash {
      size_t operator()(const ind_t& index) const {
        size_t seed = 0;
        for(const auto& it: index) // boost hash_combine.
          seed = std::hash<index_elem_t>()(it) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
      }
    };



    using storage_t = std::unordered_map<ind_t, T, hash>;

    using storage_ptr_t = std::weak_ptr<storage_t>;


    void set(const ind_t& index, const T& value) {
      storage_[index] = value;
    }

    T& get(const ind_t& index) {
      return storage_[index];
    }


    template<typename U, index_elem_t proxy_ind>
    class matrix_proxy {
      public:
        matrix_proxy(storage_t& storage, ind_t& index) : matrix_storage_{storage}, index_{index} {
        }

        auto operator[](index_elem_t elem_ind) const {
          index_[proxy_ind] = elem_ind;
          return matrix_proxy<U, proxy_ind - 1>{matrix_storage_, index_};
        }

      private:
        storage_t& matrix_storage_;
        ind_t& index_;
    };


    template<typename U>
    class matrix_proxy<U, 0> {
      public:
        matrix_proxy(storage_t& storage, ind_t& index) : matrix_storage_{storage}, index_{index} {
        }

        auto operator[](index_elem_t elem_ind) const {
          index_[0] = elem_ind;
          return matrix_storage_[index_];
        }

      private:
        storage_t& matrix_storage_;
        ind_t& index_;
    };


    matrix_proxy<void, N> operator[] (index_elem_t elem_ind) {
      ind_t index{};
      return matrix_proxy<void, N - 2>{storage_, index};
    }

    const matrix_proxy<void, N> operator[] (index_elem_t elem_ind) const {
      ind_t index{};
      return matrix_proxy<void, N - 2>{storage_, index};
    }


  private:

    storage_t storage_{};

};

int main()
{
  std::cout << "matrix version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;


  using mat_t = matrix<int, 0, 5>;
  mat_t mat;

//  auto index = std::make_tuple<int, int>(1, 1);

  mat_t::ind_t ind{{1, 1, 1, 2, 2}};
  int val = 5;
  mat.set(ind, val);

  mat[1][2][3][4][5] = 87;

  std::cout << mat.get(ind) << std::endl;
  std::cout << mat[1][2][3][4][5] << std::endl;

  return 0;
}
