#ifndef READER_H

#define READER_H

#include "constants.h"
#include <fstream>

namespace filemanager {
class reader {
  static const int BUFF_COUNT = 2;
  char buffer[BUFF_COUNT][BUFF_SIZE];
  int curr_buffer;
  std::streamoff idx;
  std::streamsize chars_to_process;
  bool finished;
  std::ifstream file;
  std::string filename;

  void flush();

public:
  reader();
  reader(const std::string &input_file);
  void open(const std::string &input_file);
  void close();
  bool is_open() const;
  bool is_finished() const;
  char get_next_char() const;
  void increment_ptr();
  std::string get_filename() const;
};
}; // namespace filemanager

#endif /* READER_H */
