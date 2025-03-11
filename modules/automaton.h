#ifndef MODULE_AUTOMATON_H
#define MODULE_AUTOMATON_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace ycc {

class automaton {
protected:
  struct node {
    std::map<char, int> transition;
    bool is_final;
    bool advance_ptr;
    int other_transition;
    std::string token;

    node(int other_transition = -1);
    node(bool is_final, bool advance_ptr, int other_transition = -1);
    node(bool is_final, bool advance_ptr, const std::string &token,
         int other_transition = -1);
  };

  enum DELETE_STATE_RETVAL { SUCCESS = 0, NOT_FOUND = 1, HAS_TRANSITIONS = 2 };

  int get_new_state();
  DELETE_STATE_RETVAL delete_state(int state);
  std::vector<node> transition_table;
  int current_state;
  std::set<int> extra_states;
  inline node &get_node(int state);
  inline const node &get_node(int state) const;

public:
  automaton();
  automaton(const std::string &transition_filename,
            const std::string &token_filename);
  void insert_string(const std::string &str, const std::string &token = "");
  void delete_string(const std::string &str);
  std::pair<bool, std::string> find(const std::string &str);
  bool process_char(char c);
  bool is_accepted() const;
  bool is_final() const;
  bool can_advance_ptr() const;
  void reset_automaton();
};

} // namespace ycc

#endif
