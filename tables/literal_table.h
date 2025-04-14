#ifndef TABLES_LITERAL_TABLE_H

#define TABLES_LITERAL_TABLE_H

#include <modules/immutable_vector.h>
#include <string>
#include <tables/base_table.h>
#include <unordered_map>
#include <utility>

namespace ycc {
class literal_table : public base_table<literal_table> {
  friend class base_table<literal_table>;

public:
  enum literal_type { NUMERIC, STRING };
  struct table_entry {
    std::string value;
    literal_type type;
    bool operator==(const table_entry &other) const {
      return value == other.value && type == other.type;
    }
    table_entry() : value(), type(static_cast<literal_type>(-1)) {};
    table_entry(const std::string &value, const literal_type &type)
        : value(value), type(type) {}
  };

  void push_back(const table_entry &entry) {
    table.push_back(entry);
    table_map[entry] = table.size() - 1;
  }
  template <typename... Args> void emplace_back(Args &&...args) {
    table.emplace_back(std::forward<Args>(args)...);
    table_map[table.back()] = table.size() - 1;
  }

  std::pair<bool, size_t> find(const std::string &value,
                               const literal_type &type) {
    auto it = table_map.find(table_entry(value, type));

    if (it == table_map.end())
      return std::make_pair(false, 0);
    return std::make_pair(true, it->second);
  }

private:
  struct table_entry_hash {
    size_t operator()(const table_entry &entry) const {
      return std::hash<std::string>()(entry.value) ^
             std::hash<int>()(entry.type);
    }
  };
  immutable_vector<table_entry> table;
  std::unordered_map<table_entry, size_t, table_entry_hash> table_map;
};
} // namespace ycc
#endif /* TABLES_LITERAL_TABLE_H */
