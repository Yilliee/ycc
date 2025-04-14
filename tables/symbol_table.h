#ifndef TABLES_SYMBOL_TABLE_H

#define TABLES_SYMBOL_TABLE_H

#include <modules/immutable_vector.h>
#include <string>
#include <tables/base_table.h>
#include <unordered_map>
#include <utility>

namespace ycc {
class symbol_table : public base_table<symbol_table> {
  friend class base_table<symbol_table>;

public:
  struct table_entry {
    std::string symbol;

    bool operator==(const table_entry &other) const {
      return symbol == other.symbol;
    }
    table_entry() = default;
    table_entry(const std::string &symbol) : symbol(symbol) {}
  };
  symbol_table() = default;

  std::pair<bool, size_t> find(const std::string &symbol) {
    auto it = table_map.find(table_entry(symbol));
    if (it == table_map.end())
      return std::make_pair(false, 0);
    return std::make_pair(true, it->second);
  }
  void push_back(const table_entry &entry) {
    table.push_back(entry);
    table_map[entry] = table.size() - 1;
  }
  template <typename... Args> void emplace_back(Args &&...args) {
    table.emplace_back(std::forward<Args>(args)...);
    table_map[table.back()] = table.size() - 1;
  }

private:
  struct table_entry_hash {
    bool operator()(const table_entry &entry) const {
      return std::hash<std::string>()(entry.symbol);
    }
  };
  immutable_vector<table_entry> table;
  std::unordered_map<table_entry, size_t, table_entry_hash> table_map;
};
} // namespace ycc
#endif /* TABLES_SYMBOL_TABLE_H */
