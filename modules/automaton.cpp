#include "automaton.h"
#include <fstream>
#include <iostream>
#include <stack>

#define is_in(ds, val) (ds.find(val) != ds.end())

using namespace ycc;

using std::string, std::getline, std::vector, std::pair, std::ifstream,
    std::cerr;

inline automaton::node &automaton::get_node(int state) {
  return transition_table[state];
}

inline const automaton::node &automaton::get_node(int state) const {
  return transition_table[state];
}

vector<string> csv_split(const string &line) {
  vector<string> cells;
  size_t st = 0, en;
  bool escaped = false;

  while ((en = line.find(',', st)) != string::npos) {
    string token = std::move(line.substr(st, en - st));
    size_t token_size = token.size();
    if (escaped) {
      cells.back() += token;
      escaped = false;
      st = en + 1;
      continue;
    }
    if (!token.empty() && token[token_size - 1] == '\\' &&
        (token_size == 1 || token[token_size - 2] != '\\')) {
      escaped = true;
    }
    cells.emplace_back(std::move(token));
    st = en + 1;
  }
  if (escaped)
    cells.back() += line.substr(st);
  else
    cells.push_back(line.substr(st));

  return cells;
}

automaton::node::node(int other_transition)
    : is_final(false), advance_ptr(true), other_transition(other_transition),
      token("") {}

automaton::node::node(bool is_final, bool advance_ptr, int other_transition)
    : is_final(is_final), advance_ptr(advance_ptr),
      other_transition(other_transition), token("") {}

automaton::node::node(bool is_final, bool advance_ptr, const string &token,
                      int other_transition)
    : is_final(is_final), advance_ptr(advance_ptr),
      other_transition(other_transition), token(token) {}

automaton::automaton()
    : transition_table(1), current_state(0), extra_states() {}

automaton::automaton(const string &transition_filename,
                     const string &token_filename)
    : transition_table(1), current_state(0), extra_states() {
  ifstream transition_file(transition_filename);

  if (!transition_file.is_open())
    throw std::runtime_error("Could not open file: " + transition_filename);

  string line;
  while (getline(transition_file, line)) {
    vector<string> cells = csv_split(line);

    // Each line contains 3 parts (index,char,dest_idx) where char can be null
    if (cells.size() < 3) {
      cerr << "[automaton] Found a line with less than 3 tokens in "
           << transition_filename << ": " << line << '\n';
      cerr << "[automaton] Skipping this line\n";
      continue;
    }

    int index = std::stoi(cells[0]);
    string c = cells[1];
    int dest_idx = std::stoi(cells[2]);

    size_t required_size =
        std::max(static_cast<size_t>(index), static_cast<size_t>(dest_idx)) + 1;
    if (transition_table.size() < required_size) {
      transition_table.resize(required_size);
    }

    if (c.empty())
      get_node(index).other_transition = dest_idx;
    else
      get_node(index).transition[c[0]] = dest_idx;
  }

  ifstream token_file(token_filename);

  if (!token_file.is_open())
    throw std::runtime_error("Could not open file: " + token_filename);

  // Process the token file: expected columns are index, advance_ptr, token.
  // Here we mark each node as final.
  while (getline(token_file, line)) {
    std::cout << "Token line: " << line << std::endl;
    vector<string> cells = csv_split(line);

    if (cells.size() < 3) {
      cerr << "[automaton] Found a token line with less than 3 tokens in "
           << token_filename << ": " << line << '\n';
      cerr << "[automaton] Skipping this line\n";
      continue;
    }

    int index = std::stoi(cells[0]);
    bool advance_ptr = (cells[1] == "1");
    string token = cells[2];

    if (transition_table.size() <= static_cast<size_t>(index)) {
      transition_table.resize(index + 1);
    }

    get_node(index).advance_ptr = advance_ptr;
    get_node(index).is_final = true;
    get_node(index).token = token;
  }
}

int automaton::get_new_state() {
  int retval = -1;
  if (!extra_states.empty()) {
    retval = *extra_states.begin();
    extra_states.erase(extra_states.begin());
    return retval;
  }

  transition_table.emplace_back();
  return transition_table.size() - 1;
}

void automaton::insert_string(const std::string &str,
                              const std::string &token) {
  size_t n = str.size();
  int state = 0;
  for (size_t i = 0; i < n; i++) {
    // Add a new transition if it doesn't already exist
    if (!is_in(get_node(state).transition, str[i])) {
      /*
       * If the current state is the final state, then
       * node.transitions should be empty.
       * We create 2 new states instead of 1, one pointing
       * to the actual transition of str[i] on node,
       * and the other dealing with the else case to state
       * that the current token is finished.
       */
      if (get_node(state).is_final) {
        int new_state = get_new_state();
        get_node(new_state) = node(true, false, get_node(state).token);
        get_node(state) = node(false, true, new_state);
      }

      // Add the transition for the actual character with
      // the default params
      int new_state = get_new_state();
      get_node(new_state) = node();
      get_node(state).transition[str[i]] = new_state;
    }
    state = get_node(state).transition[str[i]];
  }

  // Is the current state a leaf node (requirement for being a final state)
  if (get_node(state).transition.empty()) {
    // Mark the state final if it isn't already marked as such
    get_node(state).is_final = true;
    get_node(state).token = token;
    return;
  }

  /*
   * If the current state is not previously marked as final and we can
   * transition to other states from this state, then create a state for others
   * transition from the current state, to represent the end of the current
   * string.
   */
  // Do we already have the transition we're about to make?
  if (get_node(state).other_transition != -1) {
    int other_state = get_node(state).other_transition;

    // Do we already have a final state with the correct token ?
    if (get_node(other_state).is_final &&
        (token.empty() || get_node(other_state).token == token))
      return;

    if (!token.empty() && get_node(other_state).token != token) {
      cerr << "[automaton] Token mismatch for state: " << other_state
           << ", string: " << str << '\n';
      cerr << "[automaton] Existing token: " << get_node(other_state).token
           << ", new token: " << token << '\n';
      cerr << "[automaton] Overriding old token with new token...\n";
      get_node(other_state).token = token;
      return;
    }
    if (!get_node(other_state).transition.empty()) {
      cerr << "[automaton] Found an invalid state: " << state
           << " with an other transition to a non-final state.\n";
      cerr << "[automaton] This most likely occurred due to loading a premade "
              "automaton with this invalid state.\n";
      cerr << "[automaton] Skipping the insertion of the string: " << str
           << "...\n";
      cerr << "[automaton] If you want to insert this string, please delete "
              "the invalid state first.\n";
      return;
    }
    get_node(other_state).is_final = true;
    get_node(other_state).token = token;
    return;
  }

  /*
   * Create the state to transition to in case no other transition is found.
   * The new state is a final state and we won't advance the pointer for this
   * transition
   */
  int new_state = get_new_state();
  get_node(new_state) =
      node(true, false, token); // Store the token in the new final state
  get_node(state).other_transition = new_state;
}

automaton::DELETE_STATE_RETVAL automaton::delete_state(int state) {
  if (state < 0 || static_cast<size_t>(state) >= transition_table.size())
    return DELETE_STATE_RETVAL::NOT_FOUND;
  if (is_in(extra_states, state))
    return DELETE_STATE_RETVAL::SUCCESS;

  if (!get_node(state).transition.empty())
    return DELETE_STATE_RETVAL::HAS_TRANSITIONS;

  if (get_node(state).other_transition != -1)
    delete_state(get_node(state).other_transition);

  extra_states.insert(state);
  return DELETE_STATE_RETVAL::SUCCESS;
}

void automaton::delete_string(const std::string &str) {
  std::stack<int> parent_states;

  size_t n = str.size();
  int state = 0;

  for (size_t i = 0; i < n; i++) {
    if (!is_in(get_node(state).transition, str[i])) {
      return; // The string doesn't exist in the automaton
    }

    int next_state = get_node(state).transition[str[i]];
    parent_states.push(state); // Push the parent state, not the next state
    state = next_state;
  }

  // Does the word we're trying to delete exist in the automaton?
  if (parent_states.size() != str.size()) {
    return;
  }

  if (get_node(state).other_transition != -1) {
    delete_state(get_node(state).other_transition);
    get_node(state).other_transition = -1;
  }
  get_node(state).is_final = false;

  for (int i = str.size() - 1; i >= 0; i--) {
    if (parent_states.empty())
      break;

    int parent_state = parent_states.top();
    parent_states.pop();

    int child_state = get_node(parent_state).transition[str[i]];
    // If child has some other string dependent on it, we can't remove this node
    if (!get_node(child_state).transition.empty() ||
        get_node(child_state).other_transition != -1)
      break;

    if (delete_state(child_state) == DELETE_STATE_RETVAL::SUCCESS)
      get_node(parent_state).transition.erase(str[i]);
    else {
      cerr << "[automaton] Failed to delete state: " << child_state << '\n';
      break;
    }
  }
}

pair<bool, string> automaton::find(const std::string &str) {
  size_t i = 0, n = str.size();
  int state = 0;
  while (i < n && state != -1) {
    if (!is_in(get_node(state).transition, str[i]))
      break;
    state = get_node(state).transition[str[i]];
    i++;
  }

  if (i < n)
    return {false, ""};

  if (get_node(state).is_final)
    return {true, get_node(state).token};

  int other_state = get_node(state).other_transition;
  if (other_state == -1 || !get_node(other_state).is_final) {
    if (other_state != -1)
      cerr << "[automaton] Found an invalid state: " << state
           << " with an other transition to a non-final state.\n";
    return {false, ""};
  }
  return {true, get_node(other_state).token};
}

bool automaton::process_char(char c) {
  if (is_in(get_node(current_state).transition, c)) {
    current_state = get_node(current_state).transition[c];
    return true;
  }
  if (get_node(current_state).other_transition != -1) {
    current_state = get_node(current_state).other_transition;
    return true;
  }

  return false;
}

bool automaton::is_accepted() const {
  int other_state = get_node(current_state).other_transition;
  return this->is_final() ||
         (other_state != -1 && get_node(other_state).is_final);
}

bool automaton::is_final() const { return get_node(current_state).is_final; }

bool automaton::can_advance_ptr() const {
  return get_node(current_state).advance_ptr;
}

void automaton::reset_automaton() { current_state = 0; }
