#include "writer.h"
#include "constants.h"
#include <ios>

using namespace filemanager;

writer::writer() : curr_buffer(0), idx(0), chars_to_process(0) {}

writer::writer(const std::string &input_file)
    : curr_buffer(0), idx(0), chars_to_process(0), filename(input_file) {
  open(input_file);
}

void writer::flush(int buffnum, std::streamoff chars_count) {
  if (chars_to_process == 0)
    return;

  file.write(buffer[buffnum], chars_count);
  chars_to_process -= chars_count;
}

void writer::open(const std::string &input_file) {
  close();
  file.open(input_file);
  filename = input_file;
}

void writer::close() {
  int buffers_used = chars_to_process / BUFF_SIZE;
  if (chars_to_process % BUFF_SIZE == 0)
    buffers_used--;

  curr_buffer -= buffers_used;
  if (curr_buffer < 0)
    curr_buffer += BUFF_COUNT;

  while (chars_to_process > 0) {
    std::streamoff chars_count = chars_to_process;
    if (chars_count > BUFF_SIZE)
      chars_count = BUFF_SIZE;

    flush(curr_buffer, chars_count);
    curr_buffer = (curr_buffer + 1) % BUFF_COUNT;
  }
  file.close();
  filename.clear();
}

bool writer::is_open() const { return file.is_open(); }

void writer::put_next_char(char c) {
  if (!is_open()) {
    throw std::runtime_error("File not open or already finished");
  }

  buffer[curr_buffer][idx] = c;
  idx++;
  chars_to_process++;

  if (idx == BUFF_SIZE) {
    idx = 0;

    curr_buffer = (curr_buffer + 1) % BUFF_COUNT;
    if (chars_to_process == BUFF_COUNT * BUFF_SIZE)
      flush(curr_buffer, BUFF_SIZE);
  }
}

std::string writer::get_filename() const { return filename; }
