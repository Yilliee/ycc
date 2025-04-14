#ifndef MODULES_IMMUTABLE_VECTOR_H

#define MODULES_IMMUTABLE_VECTOR_H

#include <vector>
namespace ycc {
template <typename T> class immutable_vector {
private:
  std::vector<T> data; // Underlying storage
public:
  using value_type = T; // Define value_type as the element type
  immutable_vector() = default;
  immutable_vector(size_t size);
  immutable_vector(size_t size, const T &value);
  immutable_vector(std::initializer_list<T> init);
  void push_back(const T &value);
  void pop_back();
  template <typename... Args> void emplace_back(Args &&...args);
  const T &operator[](size_t index) const;
  const T &at(size_t index) const;
  bool empty() const;
  size_t size() const;
  size_t max_size() const;
  size_t capacity() const;
  void reserve(size_t new_cap);
  void resize(size_t count);
  void resize(size_t count, const T &value);
  void clear();
  const T &back() const;
  const T &front() const;
  typename std::vector<T>::const_iterator begin() const;
  typename std::vector<T>::const_iterator end() const;
  typename std::vector<T>::const_iterator rbegin() const;
  typename std::vector<T>::const_iterator rend() const;
  typename std::vector<T>::const_iterator cbegin() const;
  typename std::vector<T>::const_iterator cend() const;
};
} // namespace ycc

#include "immutable_vector.tcc"

#endif /* MODULES_IMMUTABLE_VECTOR_H */
