#include "modules/token.h"
#include "tables/literal_table.h"
#include <lexer/lexer.h>

#include <mapping/keyword.h>
#include <mapping/operator.h>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace ycc::lexer;

const std::string lex::IDENTIFIER_TOKEN = "identifier";
const std::string lex::STRING_LITERAL_TOKEN = "op_quote";
const std::string lex::NUMERIC_LITERAL_TOKEN = "numeric_literal";
const std::string lex::TRANSITION_TABLE_FILENAME = "files/transitions.txt";
const std::string lex::TRANSITION_TOKENS_FILENAME = "files/tokens.txt";
const std::string lex::KEYWORDS_TOKENS_FILENAME = "files/keywords.txt";

lex::lex(const std::string &filename,
         const std::shared_ptr<symbol_table> &sym_table,
         const std::shared_ptr<literal_table> &lit_table)
    : reader(filename),
      tokens_automaton(TRANSITION_TABLE_FILENAME, TRANSITION_TOKENS_FILENAME),
      keywords_automaton(KEYWORDS_TOKENS_FILENAME), sym_table(sym_table),
      lit_table(lit_table) {
  if (!reader.is_open())
    throw std::runtime_error("Unable to open file for lexer: " + filename);
}

bool lex::is_stream_finished() const { return reader.is_finished(); }

ycc::token lex::get_next_token() {
  if (is_stream_finished())
    return token(token_type::ERROR, "",
                 token::locality(reader.get_filename(), line, col));

  tokens_automaton.reset_automaton();
  keywords_automaton.reset_automaton();

  bool first_transition = true;
  bool string_literal = false;
  bool kw_failed = false, tok_failed = false;
  bool firstSpace = false;
  std::ostringstream oss;

  size_t curr_line = line, curr_col = col;
  while (!reader.is_finished()) {
    char c = reader.get_next_char();

    if (isspace(c)) {
      if (c == '\n') {
        line++;
        col = 0;
      }
      if (firstSpace || first_transition) {
        reader.increment_ptr();
        curr_line = line;
        curr_col = col;
        continue;
      }
      firstSpace = true;
    } else if (firstSpace)
      firstSpace = false;

    if (!tokens_automaton.on_invalid_state())
      tokens_automaton.process_char(c);
    if (!keywords_automaton.on_invalid_state())
      keywords_automaton.process_char(c);

    kw_failed = keywords_automaton.on_invalid_state();
    tok_failed = tokens_automaton.on_invalid_state();
    if (kw_failed && tok_failed) {
      if (first_transition) {
        oss << c;
        reader.increment_ptr();
      }
      return token(token_type::ERROR, oss.str(),
                   token::locality(reader.get_filename(), curr_line, curr_col));
    }

    if ((!kw_failed && keywords_automaton.can_advance_ptr()) ||
        (!tok_failed && tokens_automaton.can_advance_ptr())) {
      oss << c;
      reader.increment_ptr();
    }

    if (!kw_failed && !tok_failed && tokens_automaton.is_final() &&
        keywords_automaton.is_final())
      throw std::runtime_error(
          "Ambiguous token: " + tokens_automaton.get_token() + " or " +
          keywords_automaton.get_token());

    if (!kw_failed && keywords_automaton.is_final()) {
      std::string curr_token = keywords_automaton.get_token();
      auto it = KEYWORDS_MAP.find(curr_token);
      if (it == KEYWORDS_MAP.end())
        throw std::runtime_error("Found an invalid keywork: " + curr_token);
      return token(token_type::KEYWORD, static_cast<int>(it->second),
                   token::locality(reader.get_filename(), curr_line, curr_col));
    }

    if (!tok_failed && tokens_automaton.is_final()) {
      std::string curr_token = tokens_automaton.get_token();

      if (curr_token == STRING_LITERAL_TOKEN) {
        string_literal = true;
        char starting_char = c;
        while (!reader.is_finished() && string_literal) {
          c = reader.get_next_char();
          col++;
          reader.increment_ptr();
          if (c == '\n') {
            line++;
            col = 0;
            break;
          }
          if (c == starting_char)
            string_literal = false;
          else
            oss << c;
        }
        if (string_literal)
          return token(
              token_type::ERROR, oss.str(),
              token::locality(reader.get_filename(), curr_line, curr_col));
        std::string literal = oss.str().substr(1);
        lit_table->emplace_back(literal, literal_table::literal_type::STRING);
        return token(
            token_type::LITERAL, lit_table->size() - 1,
            token::locality(reader.get_filename(), curr_line, curr_col));
      }
      if (curr_token == NUMERIC_LITERAL_TOKEN) {
        lit_table->emplace_back(oss.str(),
                                literal_table::literal_type::NUMERIC);
        return token(
            token_type::LITERAL, lit_table->size() - 1,
            token::locality(reader.get_filename(), curr_line, curr_col));
      }
      auto it = OPERATORS_MAP.find(curr_token);
      if (it == OPERATORS_MAP.end() && curr_token != IDENTIFIER_TOKEN)
        throw std::runtime_error(
            "Found an invalid token in tokens_automaton: " + curr_token);
      if (it != OPERATORS_MAP.end())
        return token(
            token_type::OPERATOR, static_cast<int>(it->second),
            token::locality(reader.get_filename(), curr_line, curr_col));

      std::string symbol = oss.str();
      auto [found, idx] = sym_table->find(symbol);
      if (!found) {
        sym_table->emplace_back(symbol);
        idx = sym_table->size() - 1;
      }
      return token(token_type::IDENTIFIER, idx,
                   token::locality(reader.get_filename(), curr_line, curr_col));
    }

    first_transition = false;
  }

  if (!tokens_automaton.on_invalid_state())
    tokens_automaton.force_other_transition();
  if (!keywords_automaton.on_invalid_state())
    keywords_automaton.force_other_transition();

  kw_failed = keywords_automaton.on_invalid_state();
  tok_failed = tokens_automaton.on_invalid_state();
  if ((!kw_failed && keywords_automaton.can_advance_ptr()) ||
      (!tok_failed && tokens_automaton.can_advance_ptr())) {
    reader.increment_ptr();
  }

  if (!kw_failed && keywords_automaton.is_final()) {
    std::string curr_token = keywords_automaton.get_token();
    auto it = KEYWORDS_MAP.find(curr_token);
    if (it == KEYWORDS_MAP.end())
      throw std::runtime_error("Found an invalid keywork: " + curr_token);
    return token(token_type::KEYWORD, static_cast<int>(it->second),
                 token::locality(reader.get_filename(), curr_line, curr_col));
  }
  if (!tok_failed && tokens_automaton.is_final()) {
    std::string curr_token = tokens_automaton.get_token();
    auto it = OPERATORS_MAP.find(curr_token);
    if (it == OPERATORS_MAP.end() && curr_token != IDENTIFIER_TOKEN)
      throw std::runtime_error("Found an invalid token in tokens_automaton: " +
                               curr_token);
    if (it != OPERATORS_MAP.end())
      return token(token_type::OPERATOR, static_cast<int>(it->second),
                   token::locality(reader.get_filename(), curr_line, curr_col));

    std::string symbol = oss.str();
    auto [found, idx] = sym_table->find(symbol);
    if (!found) {
      sym_table->emplace_back(symbol);
      idx = sym_table->size() - 1;
    }
    return token(token_type::IDENTIFIER, idx,
                 token::locality(reader.get_filename(), curr_line, curr_col));
  }

  return token(token_type::ERROR, oss.str(),
               token::locality(reader.get_filename(), curr_line, curr_col));
}
