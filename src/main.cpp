#include <iostream>

#include "../inc/ver.h"


#include <unordered_map>
#include <tuple>
#include <array>
#include <memory>

//template <size_t ind, typename T, typename... Tail>
//struct gen_tuple_t {
//  using type = typename gen_tuple_t<ind - 1, T, T, Tail...>::type;
//};

//template <typename T, typename... Tail>
//struct gen_tuple_t<0, T, Tail...> {
//  using type = std::tuple<Tail...>;
//};

template <size_t N, typename T>
struct gen_tuple_t {
  using type = decltype(std::tuple_cat(std::declval<std::tuple<std::size_t>>(),
                                       std::declval<typename gen_tuple_t<N - 1, T>::type>()));
};

template<typename T>
struct gen_tuple_t<1, T> {
  using type = std::tuple<T>;
};

//template <typename... Args>
//auto t2a(Args... elems) {
//  std::array<size_t, sizeof...(elems)> arr = {elems...};
//  return arr;
//}

template<typename T, T default_val, size_t N>
class matrix {
  public:
    using value_t = T;
    using index_elem_t = size_t;
    using index_t = typename gen_tuple_t<N, index_elem_t>::type;

//    using ind_t = std::array<index_elem_t, N>;

    // boost hash_combine.
    index_elem_t hash_combine(index_elem_t elem, index_elem_t seed) {
      return std::hash<index_elem_t>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }



    struct hash {
      size_t operator()(const index_t& index) const {
        size_t seed = 0;
//        std::array<index_elem_t, N> arr = t2a<index_t>(index);
//        for(auto it: arr)
          return seed;
      }
    };


    using storage_t = std::unordered_map<index_t, T, hash>;

    using storage_ptr_t = std::weak_ptr<storage_t>;


//    void set(const ind_t& index, const T& value) {
//      storage_[index] = value;
//    }

//    T& get(const ind_t& index) {
//      return storage_[index];
//    }


    template<typename F, index_elem_t ind>
    class matrix_proxy {
      public:
        matrix_proxy(storage_t& storage, const F& index) : matrix_storage_{storage}, index_{index} {
        }

        auto operator[](index_elem_t elem_ind) {
          auto nextIndex = std::tuple_cat(index_, std::make_tuple(elem_ind));
          return matrix_proxy<decltype(nextIndex), ind - 1>{matrix_storage_, nextIndex};
        }

      private:
        storage_t& matrix_storage_;
        F index_;
    };


    template<typename F>
    class matrix_proxy<F, 0> {
      public:
        matrix_proxy(storage_t& storage, F& index) : matrix_storage_{storage}, index_{index} {
        }

        auto& operator = (const value_t& value) {
//          if(value == default_val) {
//            auto it = matrix_storage_->erase(index_);
//            return default_val;
//          }
          index_t ind{};
          return matrix_storage_[ind] = value;
        }

        operator const value_t& () const {
//          auto it = matrix_storage_->find(index_);
//          return it == matrix_storage_->cend() ? default_val : it->second;
          auto ind = std::make_tuple<index_elem_t, index_elem_t>(0,0);
          return matrix_storage_[ind];
        }

      private:
        storage_t& matrix_storage_;
        F index_;
        value_t tmp;
    };

    auto operator[] (index_elem_t elem_ind) {
      auto index = std::make_tuple(elem_ind);
      return matrix_proxy<decltype (index), N - 1>{storage_, index};
    }

    auto operator[] (index_elem_t elem_ind) const {
      auto index = std::make_tuple(elem_ind);
      return matrix_proxy<decltype (index), N - 1>{storage_, index};
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


  using mat_t = matrix<int, 0, 2>;
  mat_t mat;

////  auto index = std::make_tuple<int, int>(1, 1);

//  mat_t::ind_t ind{{1, 1, 1, 2, 2}};
//  int val = 5;
//  mat.set(ind, val);

//  mat[1][2][3][4][5] = val;

////  std::cout << mat.get(ind) << std::endl;
//  std::cout << mat[1][2][3][4][5] << std::endl;


  for (auto i = 0; i < 10; ++i) {
      mat[i][i] = i;
      mat[i][i] = i;
  }

  for (auto i = 1; i < 9; ++i) {
      for (auto j = 1; j < 9; ++j)
          std::cout << mat[i][j] << " ";
      std::cout << std::endl;
  }

  return 0;
}
