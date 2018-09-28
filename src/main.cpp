#include <iostream>

#include "../inc/ver.h"

#include <array>
#include <tuple>
#include <unordered_map>

template <size_t N, typename T>
struct gen_tuple_t {
  using type = decltype(std::tuple_cat(std::declval<std::tuple<std::size_t>>(),
                                       std::declval<typename gen_tuple_t<N - 1, T>::type>()));
};

template<typename T>
struct gen_tuple_t<1, T> {
  using type = std::tuple<T>;
};

template<typename T, T default_val, size_t N>
class matrix {
    using index_t = typename gen_tuple_t<N, size_t>::type;

    struct hash {
      template <size_t... Indices>
      auto t2a(const index_t& tuple, std::index_sequence<Indices...>) const {
        std::array<size_t, std::tuple_size<index_t>::value> arr{{std::get<Indices>(tuple)...}};
        return arr;
      }

      size_t operator()(const index_t& tuple) const {
        size_t seed = 0;
        // boost hash_combine.
        for(auto it: t2a(tuple, std::make_index_sequence<N>{}))
          seed ^= std::hash<size_t>()(it) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
      }
    };

    using storage_t = std::unordered_map<index_t, T, hash>;

    template<typename F, size_t ind>
    class matrix_proxy {
      public:
        matrix_proxy(storage_t& storage, const F& index) : storage_{storage}, index_{index} {
        }

        auto operator[](size_t elem_ind) {
          auto nextIndex = std::tuple_cat(index_, std::make_tuple(elem_ind));
          return matrix_proxy<decltype(nextIndex), ind - 1>{storage_, nextIndex};
        }

      private:
        storage_t& storage_;
        F index_;
    };

    template<typename F>
    class matrix_proxy<F, 0> {
      public:
        matrix_proxy(storage_t& storage, F& index) : storage_{storage}, index_{index} {
        }

        auto& operator = (const T& value) {
          if(value == default_val) {
            storage_.erase(index_);
            return default_val_;
          }
          return storage_[index_] = value;
        }

        operator const T& () const {
          auto it = storage_.find(index_);
          return it == storage_.cend() ? default_val_ : it->second;
        }

      private:
        storage_t& storage_;
        F index_;
        T default_val_{default_val};
    };

  public:

    matrix() = default;

//    explicit matrix(size_type size) : size_(size), capacity_(size) {
//      storage_ = std::make_unique<storage_type>();
//      data_ = storage_->allocate(size_);
//      for(size_type i = 0; i < size_; ++i)
//        storage_->construct(&data_[i]);
//    }

//    matrix(size_type size, T value) : size_(size), capacity_(size) {
//      storage_ = std::make_unique<storage_type>();
//      data_ = storage_->allocate(size_);
//      for(size_type i = 0; i < size_; ++i)
//        storage_->construct(&data_[i], value);
//    }

//    matrix(const std::initializer_list<T>& mat) : size_(mat.size()), capacity_(mat.size()) {
//      storage_ = std::make_unique<storage_type>();
//      data_ = storage_->allocate(size_);
//      for(size_type i = 0; i < mat.size(); ++i)
//        storage_->construct(&data_[i], *(mat.begin() + i));
//    }

    matrix(const matrix& mat) = default;

    matrix(matrix&& mat) noexcept {
      mat.swap(*this);
    }

    matrix& operator = (matrix const& mat) {
      matrix<T, default_val, N> tmp(mat);
      tmp.swap(*this);
      return *this;
    }

    matrix& operator = (matrix&& mat) noexcept {
      mat.swap(*this);
      return *this;
    }

    ~matrix() = default;

    void swap(matrix& other) {
      std::swap(storage_, other.storage_);
    }

    auto operator[] (size_t elem_ind) {
      auto index = std::make_tuple(elem_ind);
      return matrix_proxy<decltype (index), N - 1>{storage_, index};
    }

    auto operator[] (size_t elem_ind) const {
      auto index = std::make_tuple(elem_ind);
      return matrix_proxy<decltype (index), N - 1>{storage_, index};
    }

    size_t size() {
      return storage_.size();
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


  for(auto i = 0; i < 10; ++i) {
      mat[i][i] = i;
      mat[i][9 - i] = i;
  }

  ((mat[1][1] = 314) = 0) = 3;

  for(auto i = 1; i < 9; ++i) {
      for(auto j = 1; j < 9; ++j)
          std::cout << mat[i][j] << " ";
      std::cout << std::endl;
  }

  std::cout << mat.size() << std::endl;

  return 0;
}
