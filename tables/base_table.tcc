#ifdef TABLES_BASE_TABLE_H

template <class der> ycc::base_table<der>::base_table() {
  using table_t = std::remove_reference_t<decltype(((der *)nullptr)->table)>;

  static_assert(
      is_valid_table<table_t>::value,
      "Invalid table type:\n"
      "  - Must provide: emplace_back(value_type)\n"
      "  - Must provide: at(size_t)\n"
      "  - Must provide: operator[](size_t)\n"
      "  - Must provide: begin()\n"
      "  - Must provide: end()\n"
      "  - Must provide: cbegin()\n"
      "  - Must provide: cend()\n"
      "  - Must provide: rbegin()\n"
      "  - Must provide: rend()\n"
      "  - And its value_type must not be the same as the table type itself");
}

template <class der> auto &ycc::base_table<der>::operator[](std::size_t idx) {
  return static_cast<der *>(this)->table[idx];
}

template <class der> auto &ycc::base_table<der>::at(std::size_t idx) {
  return static_cast<der *>(this)->table.at(idx);
}

template <class der> auto ycc::base_table<der>::begin() {
  static_cast<der *>(this)->table.begin();
}

template <class der> auto ycc::base_table<der>::end() {
  static_cast<der *>(this)->table.end();
}

template <class der> auto ycc::base_table<der>::rbegin() {
  static_cast<der *>(this)->table.rbegin();
}

template <class der> auto ycc::base_table<der>::rend() {
  static_cast<der *>(this)->table.rend();
}

template <class der> auto ycc::base_table<der>::cbegin() const {
  static_cast<const der *>(this)->table.cbegin();
}

template <class der> auto ycc::base_table<der>::cend() const {
  static_cast<const der *>(this)->table.cend();
};

template <class der> std::size_t ycc::base_table<der>::size() const {
  return static_cast<const der *>(this)->table.size();
}

#endif /* TABLES_BASE_TABLE_H */
