#include "trie_automaton.h"
#include "automaton.h"
#include <fstream>

using namespace ycc;

trie_automaton::trie_automaton(const std::string &filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("[trie_automaton] Could not open file: " +
                             filename);
    return;
  }

  std::string line;
  while (getline(file, line)) {
    std::string word, token;
    size_t st = 0, en;
    bool extra = false;
    do {
      en = line.find(',', st);
      if (en != std::string::npos)
        word += line.substr(st, en - st);
      extra = (word.back() == '\\' &&
               (word.size() == 1 || word[word.size() - 2] != '\\'));

      st = en + 1;
    } while (en != std::string::npos && extra);

    token = line.substr(st);
    automaton::insert_string(word, token);
  }
}
