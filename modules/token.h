#ifndef MODULES_TOKEN_H

#define MODULES_TOKEN_H

#include <optional>
#include <string>

namespace ycc {
enum token_type { ERROR, IDENTIFIER, KEYWORD, OPERATOR, LITERAL };
struct token {
  struct locality {
    std::string file;
    int line;
    int col;
    locality() : file(), line(-1), col(-1) {};
    locality(const std::string &file, int line, int col)
        : file(file), line(line), col(col) {}
  };
  token_type type;
  std::optional<std::string> value;
  size_t idx;
  std::optional<locality> loc;
  token(token_type type, size_t idx, std::optional<locality> loc = std::nullopt)
      : type(type), idx(idx), loc(loc) {}
  token(token_type type, const std::string &str,
        std::optional<locality> loc = std::nullopt)
      : type(type), value(str), idx(0), loc(loc) {}
};
} // namespace ycc
#endif /* MODULES_TOKEN_H */
