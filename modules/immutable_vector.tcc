#ifdef MODULES_IMMUTABLE_VECTOR_H

template <typename T>
ycc::immutable_vector<T>::immutable_vector(size_t size) : data(size){};

template <typename T>
ycc::immutable_vector<T>::immutable_vector(size_t size, const T &value)
    : data(size, value){};

template <typename T>
ycc::immutable_vector<T>::immutable_vector(std::initializer_list<T> init)
    : data(init){};

template <typename T> void ycc::immutable_vector<T>::push_back(const T &value) {
  data.push_back(value);
}

template <typename T> void ycc::immutable_vector<T>::pop_back() {
  data.pop_back();
}

template <typename T>
template <typename... Args>
void ycc::immutable_vector<T>::emplace_back(Args &&...args) {
  data.emplace_back(std::forward<Args>(args)...);
}

template <typename T>
const T &ycc::immutable_vector<T>::operator[](size_t index) const {
  return data[index];
}

template <typename T>
const T &ycc::immutable_vector<T>::at(size_t index) const {
  return data.at(index);
}

template <typename T> bool ycc::immutable_vector<T>::empty() const {
  return data.empty();
}

template <typename T> size_t ycc::immutable_vector<T>::size() const {
  return data.size();
}

template <typename T> size_t ycc::immutable_vector<T>::max_size() const {
  return data.max_size();
}

template <typename T> void ycc::immutable_vector<T>::reserve(size_t new_cap) {
  data.reserve(new_cap);
}

template <typename T> size_t ycc::immutable_vector<T>::capacity() const {
  return data.capacity();
}

template <typename T> void ycc::immutable_vector<T>::resize(size_t count) {
  data.resize(count);
}

template <typename T>
void ycc::immutable_vector<T>::resize(size_t count, const T &value) {
  data.resize(count, value);
}

template <typename T> const T &ycc::immutable_vector<T>::back() const {
  return data.back();
}

template <typename T> const T &ycc::immutable_vector<T>::front() const {
  return data.front();
}

template <typename T> void ycc::immutable_vector<T>::clear() { data.clear(); }

template <typename T>
typename std::vector<T>::const_iterator
ycc::immutable_vector<T>::begin() const {
  return data.begin();
}

template <typename T>
typename std::vector<T>::const_iterator ycc::immutable_vector<T>::end() const {
  return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator
ycc::immutable_vector<T>::rbegin() const {
  return data.rbegin();
}

template <typename T>
typename std::vector<T>::const_iterator ycc::immutable_vector<T>::rend() const {
  return data.rend();
}

template <typename T>
typename std::vector<T>::const_iterator
ycc::immutable_vector<T>::cbegin() const {
  return data.cbegin();
}

template <typename T>
typename std::vector<T>::const_iterator ycc::immutable_vector<T>::cend() const {
  return data.cend();
}

#endif
