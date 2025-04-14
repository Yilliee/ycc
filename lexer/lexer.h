#ifndef LEXER_LEXER_H

#include <filemanager/reader.h>
#include <memory>
#include <modules/automaton.h>
#include <modules/token.h>
#include <modules/trie_automaton.h>
#include <string>
#include <tables/literal_table.h>
#include <tables/symbol_table.h>

#define LEXER_LEXER_H

namespace ycc {
namespace lexer {
class lex {
  static const std::string IDENTIFIER_TOKEN;
  static const std::string STRING_LITERAL_TOKEN;
  static const std::string NUMERIC_LITERAL_TOKEN;
  static const std::string TRANSITION_TABLE_FILENAME;
  static const std::string TRANSITION_TOKENS_FILENAME;
  static const std::string KEYWORDS_TOKENS_FILENAME;

  filemanager::reader reader;
  automaton tokens_automaton;
  trie_automaton keywords_automaton;

  std::shared_ptr<symbol_table> sym_table;
  std::shared_ptr<literal_table> lit_table;
  size_t line;
  size_t col;

public:
  lex(const std::string &filename,
      const std::shared_ptr<symbol_table> &sym_table,
      const std::shared_ptr<literal_table> &lit_table);
  token get_next_token();
  bool is_stream_finished() const;
};
}; // namespace lexer
}; // namespace ycc

#endif // LEXER_LEXER_H
