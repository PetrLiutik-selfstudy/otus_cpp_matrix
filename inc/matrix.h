#pragma once

#include <unordered_map>

#include "../inc/utils.h"

namespace mat {

/**
 * @brief Шаблон N-мерной матрицы.
 * @tparam T - тип элемента матрицы.
 * @tparam empty_val - значение пустого элемента матрицы.
 * @tparam N - размерность матрицы (по умолчанию 2).
 */
template<typename T, T empty_val, size_t N = 2>
class matrix {
    /// Тип кортежа, содержащего все индексы матрицы.
    using index_t = typename utils::gen_tuple_t<N, size_t>::type;
    /// Тип хранилища элементов матрицы.
    using storage_t = std::unordered_map<index_t, T, utils::hash<index_t>>;

    /**
     * @brief Шаблон прокси для имитации N-мерной матрицы.
     * @tparam F - тип кортежа, содержащего индексы для обращения к элементу матрицы.
     * @tparam ind - номер индекса по порядку.
     */
    template<typename F, size_t ind>
    class matrix_proxy {
      public:
        matrix_proxy(storage_t& storage, const F& index) : storage_{storage}, index_{index} {
        }

        /**
         * @brief Оператор [].
         * @param elem_ind - индекс элемента по текущему "измерению" матрицы.
         * @return следующий прокси.
         */
        auto operator [] (size_t elem_ind) {
          auto nextIndex = std::tuple_cat(index_, std::make_tuple(elem_ind));
          return matrix_proxy<decltype(nextIndex), ind - 1>{storage_, nextIndex};
        }

      private:
        storage_t& storage_;
        F index_;
    };

    /**
     * @brief Шаблон прокси для имитации N-мерной матрицы (конец рекурсии).
     * @tparam F - тип кортежа, содержащего индексы для обращения к элементу матрицы.
     */
    template<typename F>
    class matrix_proxy<F, 0> {
      public:
        matrix_proxy(storage_t& storage, F& index) : storage_{storage}, index_{index} {
        }

        /**
         * @brief Оператор присваивания.
         * @param value - ссылка на объект с присваиваемым значением.
         * @return ссылка на элемент матрицы.
         */
        auto& operator = (const T& value) {
          if(value == empty_val) {
            storage_.erase(index_);
            return empty_val_;
          }
          return storage_[index_] = value;
        }

        /**
         * @brief Оператор const T & возвращает const ссылку на элемент матрицы
         *        вместо ссылки на прокси.
         * @return const ссылка на элемент матрицы.
         */
        operator const T & () const {
          auto it = storage_.find(index_);
          return it == storage_.cend() ? empty_val_ : it->second;
        }

      private:
        storage_t& storage_;
        F index_;
        T empty_val_{empty_val};
    };

  public:
    /// Тип кортежа, содержащего все индексы и значение элемента матрицы.
    using index_val_t = typename utils::gen_tuple_t<N + 1, size_t>::type;

    matrix() = default;

    /**
     * @brief Конструктор с кортежем с индексами и значениям элемента.
     * @param index_val - кортеж с индексами и значением элемента.
     */
    matrix(const index_val_t& index_val) {
      auto index = utils::get_subtuple(index_val, std::make_index_sequence<N>{});
      auto value = std::get<N>(index_val);
      if(value != empty_val) {
        storage_[index] = value;
      }
    }

    /**
     * @brief Конструктор со списком инициализации из кортежей с индексами и значениями.
     * @param list - список инициализации.
     */
    matrix(const std::initializer_list<index_val_t>& list) {
      for(const auto& it: list) {
        auto index = utils::get_subtuple(it, std::make_index_sequence<N>{});
        auto value = std::get<N>(it);
        if(value != empty_val) {
          storage_[index] = value;
        }
      }
    }

    matrix(const matrix& mat) = default;

    /**
     * @brief Перемещающий конструктор.
     * @param mat - иницицализирующая матрица.
     */
    matrix(matrix&& mat) noexcept {
      mat.swap(*this);
    }

    /**
     * @brief Оператор присваивания.
     * @param mat - матрица
     * @return - ссылка на данную матрицу.
     */
    matrix& operator = (matrix const& mat) {
      matrix<T, empty_val, N> tmp(mat);
      tmp.swap(*this);
      return *this;
    }

    /**
     * @brief Перемещающий оператор присваивания.
     * @param mat - другая матрица.
     * @return - ссылка на данную матрицу.
     */
    matrix& operator = (matrix&& mat) noexcept {
      mat.swap(*this);
      return *this;
    }

    ~matrix() = default;

    /**
     * @brief Обмен значениями с другим объектом матрицы.
     * @param other - объект с которым производится обмен значениями.
     */
    void swap(matrix& other) {
      std::swap(storage_, other.storage_);
    }

    /**
     * @brief Оператор [].
     * @param elem_ind - индекс первого "измерения" матрицы.
     * @return прокси, имитирующий обращения по остальным N - 1 измерениям.
     */
    auto operator [] (size_t elem_ind) {
      auto index = std::make_tuple(elem_ind);
      return matrix_proxy<decltype (index), N - 1>{storage_, index};
    }

    /**
     * @brief Константный оператор [].
     * @param elem_ind - индекс первого "измерения" матрицы.
     * @return прокси, имитирующий обращения по остальным N - 1 измерениям.
     */
    auto operator [] (size_t elem_ind) const {
      auto index = std::make_tuple(elem_ind);
      return matrix_proxy<decltype (index), N - 1>{storage_, index};
    }

    /**
     * @brief Оператор равенства.
     * @param other - другой объект матрицы.
     * @return - true - объекты равны, false - объекты не равны.
     */
    bool operator == (const matrix &other) const {
      return storage_ == other.storage_;
    }

    /**
     * @brief Оператор неравенства.
     * @param other - другой объект матрицы.
     * @return - false - объекты равны, true - объекты не равны.
     */
    bool operator != (const matrix &other) const {
      return !(*this == other);
    }

    /**
     * @brief Дать число занятых элементов матрицы.
     * @return число занятых элементов матрицы.
     */
    size_t size() {
      return storage_.size();
    }

    /// Тип итератора контейнера, содержащего элементы матрицы.
    using storage_iterator_t = typename storage_t::iterator;

    /**
     * @brief Класс итератора матрицы.
     */
    struct iterator : std::iterator<std::bidirectional_iterator_tag, storage_iterator_t> {
        explicit iterator(storage_iterator_t it) : current_{it} {}

        iterator& operator ++() {
          current_++;
          return *this;
        }

        iterator operator ++(int) {
          iterator temp = *this;
          current_++;
          return temp;
        }

        iterator& operator --() {
          current_--;
          return *this;
        }

        iterator operator --(int) {
          iterator temp = *this;
          current_--;
          return temp;
        }

        auto operator *() {
          return std::tuple_cat((*current_).first, std::make_tuple((*current_).second));
        }

        bool operator == (const iterator &other) const {
          return current_ == other.current_;
        }

        bool operator != (const iterator &other) const {
          return !(*this == other);
        }

    private:
        storage_iterator_t current_{};
    };

    /**
     * @brief Дать итератор на начало матрицы.
     * @return итератор на начало матрицы.
     */
    iterator begin() {
      return iterator(storage_.begin());
    }

    /**
     * @brief Дать итератор на конец матрицы.
     * @return итератор на конец матрицы.
     */
    iterator end() {
      return iterator(storage_.end());
    }

  private:
    storage_t storage_{};
};

} // namespace mat
