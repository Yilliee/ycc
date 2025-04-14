#include <algorithm>
#include <cassert>
#include <iostream>
#include <lexer/lexer.h>
#include <mapping/keyword.h>
#include <mapping/operator.h>
#include <memory>
#include <modules/export_tokens.h>
#include <preprocessor/preprocessor.h>
#include <tables/literal_table.h>
#include <tables/symbol_table.h>

using std::shared_ptr;
using std::string, std::cout, std::cerr, std::endl;

using namespace ycc;

void printKeyword(size_t op);
void printOperator(size_t op);

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <list of input files>" << endl;
    return 1;
  }

  bool dump_tokens = false;
  bool keep_preprocessed = false;
  for (int i = 1; i < argc && (!dump_tokens || !keep_preprocessed); i++) {
    if (string(argv[i]) == "-dump-tokens")
      dump_tokens = true;
    if (string(argv[i]) == "-E")
      keep_preprocessed = true;
  }
  shared_ptr<symbol_table> sym_table = std::make_shared<symbol_table>();
  shared_ptr<literal_table> lit_table = std::make_shared<literal_table>();

  for (int i = 1; i < argc; i++) {
    string ifile = argv[i];

    string ofile = ifile;
    ofile.erase(find(ofile.rbegin(), ofile.rend(), '.').base() - ofile.begin());
    ofile += "i";

    preprocessor::preprocess_file(ifile, ofile);

    lexer::lex lexical_parser(ofile, sym_table, lit_table);

    while (!lexical_parser.is_stream_finished()) {
      auto tok = lexical_parser.get_next_token();

      if (tok.type == token_type::ERROR &&
          (!tok.value.has_value() || tok.value.value().empty())) {
        assert(lexical_parser.is_stream_finished());
        continue;
      }

      if (dump_tokens && tok.type != token_type::ERROR) {
        cout << get_token_as_string(tok) << '\n';
      }
    }

    if (!keep_preprocessed) {
      remove(ofile.c_str());
    }
  }

  return 0;
}
