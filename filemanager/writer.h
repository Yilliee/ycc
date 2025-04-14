#ifndef WRITER_H

#define WRITER_H

#include "constants.h"
#include <fstream>

namespace filemanager {
class writer {
  static const int BUFF_COUNT = 2;
  char buffer[BUFF_COUNT][BUFF_SIZE];
  int curr_buffer;
  std::streamoff idx;
  std::streamsize chars_to_process;
  std::ofstream file;
  std::string filename;

  void flush(int buffnum, std::streamoff chars_count);

public:
  writer();
  writer(const std::string &input_file);
  void open(const std::string &input_file);
  void close();
  bool is_open() const;
  void put_next_char(char c);
  std::string get_filename() const;
};
}; // namespace filemanager

#endif /* READER_H */
