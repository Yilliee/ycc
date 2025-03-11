#ifndef TRIE_AUTOMATON_H

#define TRIE_AUTOMATON_H

#include "automaton.h"

namespace ycc {
class trie_automaton : public automaton {
public:
  trie_automaton() = default;
  trie_automaton(const std::string &filename);
};
} // namespace ycc
#endif // !##ifndef TRIE_AUTOMATON_H
