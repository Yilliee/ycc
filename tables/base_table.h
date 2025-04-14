#ifndef TABLES_BASE_TABLE_H

#define TABLES_BASE_TABLE_H

#include <type_traits>
#include <utility>

namespace ycc {
template <class derived> class base_table {
  template <typename T, typename = void>
  struct is_valid_table : std::false_type {};

  template <typename T>
  struct is_valid_table<
      T, std::void_t<decltype(std::declval<T>().emplace_back(
                         std::declval<typename T::value_type>())),
                     decltype(std::declval<T>().at(0)),
                     decltype(std::declval<T>()[0]),
                     decltype(std::declval<T>().begin()),
                     decltype(std::declval<T>().end()),
                     decltype(std::declval<T>().cbegin()),
                     decltype(std::declval<T>().cend()),
                     decltype(std::declval<T>().rbegin()),
                     decltype(std::declval<T>().rend()),
                     decltype(std::declval<T>().size())>>
      : std::integral_constant<bool, !std::is_same_v<std::remove_const<T>,
                                                     typename T::value_type>> {
  };

public:
  base_table();
  auto &operator[](std::size_t idx);
  auto &at(std::size_t idx);
  auto begin();
  auto end();
  auto rbegin();
  auto rend();
  auto cbegin() const;
  auto cend() const;
  std::size_t size() const;
  template <class der = derived, typename = std::enable_if_t<std::is_same_v<
                                     std::remove_const<der>, derived>>>
  void push_back(const typename std::remove_reference_t<
                 decltype(((der *)nullptr)->table)>::value_type &elem) {
    static_cast<derived *>(this)->table.push_back(elem);
  }
  template <class... Args> void emplace_back(Args &&...args) {
    return static_cast<derived *>(this)->table.emplace_back(
        std::forward<Args>(args)...);
  }
};
} // namespace ycc

#include "base_table.tcc"

#endif /* TABLES_BASE_TABLE_H */
