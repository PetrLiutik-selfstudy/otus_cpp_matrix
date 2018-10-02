#pragma once

#include <array>
#include <tuple>

namespace utils {

/**
 * @brief Генерация типа гомогенного кортежа, содержащего N элементов типа T.
 * @tparam N - размер кортежа.
 * @tparam T - тип элементов.
 */
template <size_t N, typename T>
struct gen_tuple_t {
  using type = decltype(std::tuple_cat(std::declval<std::tuple<std::size_t>>(),
                                       std::declval<typename gen_tuple_t<N - 1, T>::type>()));
};

/**
 * @brief Генерация типа гомогенного кортежа, содержащего N элементов типа T (конец рекурсии).
 * @tparam T - тип элементов.
 */
template<typename T>
struct gen_tuple_t<1, T> {
  using type = std::tuple<T>;
};

/**
 * @brief Генерация типа кортежа при добавлении элемента в конец кортежа.
 * @tparam T - тип кортежа.
 * @tparam U - тип добавляемого элемента.
 */
template<typename T, typename U>
struct append_tuple_t {
  using type = decltype(std::tuple_cat(std::declval<T>(), std::declval<std::tuple<U>>()));
};

/**
 * @brief Дать подкортеж данного кортежа.
 * @tparam T - тип кортежа.
 * @tparam Indices - индексы элементов кортежа, которые необходимо поместить в подкортеж.
 */
template <typename T, size_t... Indices>
auto get_subtuple(const T& tuple, std::index_sequence<Indices...>) {
  auto subtuple = std::make_tuple(std::get<Indices>(tuple)...);
  return subtuple;
}

/**
 * @brief Функтор для вычисления хэша от значения элементов содержащихся в кортеже.
 * @tparam T - тип кортежа.
 */
template<typename T>
struct hash {
  static constexpr size_t len = std::tuple_size<T>::value;

  /**
   * @brief Конвертор кортежа в std::array.
   * @tparam Indices - индексы элементов кортежа, которые необходимо поместить в массив.
   * @param tuple - кортеж.
   */
  template <size_t... Indices>
  auto t2a(const T& tuple, std::index_sequence<Indices...>) const {
    std::array<size_t, len> arr{{std::get<Indices>(tuple)...}};
    return arr;
  }

  /**
   * @brief Собственно само вычисление хэша (заимствовано из boost hash_combine).
   * @param tuple - кортеж.
   */
  size_t operator() (const T& tuple) const {
    size_t seed = 0;
    // boost hash_combine.
    for(auto it: t2a(tuple, std::make_index_sequence<len>{}))
      seed ^= std::hash<size_t>()(it) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

} // namespace utils
