#include "reader.h"

using namespace filemanager;

reader::reader()
    : curr_buffer(0), idx(0), chars_to_process(0), finished(false) {}

reader::reader(const std::string &input_file)
    : curr_buffer(0), idx(0), chars_to_process(0), finished(false) {
  open(input_file);
}

void reader::flush() { idx = 0; }

void reader::open(const std::string &input_file) {
  flush();
  file.open(input_file);
  for (int i = 0; i < BUFF_COUNT; i++) {
    int buff_num = (curr_buffer + i) % BUFF_COUNT;
    file.read(buffer[buff_num], BUFF_SIZE);
    chars_to_process += file.gcount();

    if (file.gcount() < BUFF_SIZE) {
      buffer[buff_num][file.gcount()] = '\0';
      finished = true;
      break;
    } else
      finished = false;
  }
}

void reader::close() { file.close(); }

bool reader::is_open() const { return file.is_open(); }

bool reader::is_finished() const { return finished && (chars_to_process == 0); }

char reader::get_next_char() {
  if (!is_open() || is_finished()) {
    throw std::runtime_error("File not open or already finished");
  }

  char retval = buffer[curr_buffer][idx];
  idx++;
  chars_to_process--;

  if (idx == BUFF_SIZE && !finished) {
    flush();

    file.read(buffer[curr_buffer], BUFF_SIZE);
    chars_to_process += file.gcount();
    if (file.gcount() < BUFF_SIZE) {
      buffer[curr_buffer][file.gcount()] = '\0';
      finished = true;
    }

    curr_buffer = (curr_buffer + 1) % BUFF_COUNT;
  }

  return retval;
}
