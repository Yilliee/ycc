#include <filemanager/reader.h>
#include <filemanager/writer.h>
#include <filesystem>
#include <iostream>
#include <preprocessor/preprocessor.h>
#include <queue>

using namespace ycc;

using filemanager::reader, filemanager::writer;
using std::cerr, std::endl;
using std::filesystem::path, std::filesystem::remove,
    std::filesystem::filesystem_error;

inline void skip_till_comment_start(reader &rdr, writer &wrtr,
                                    bool &single_line_comment,
                                    bool &multi_line_comment) {
  bool potentially_comment = false;

  while (!rdr.is_finished() && !(single_line_comment || multi_line_comment)) {
    char ch = rdr.get_next_char();
    rdr.increment_ptr();

    if ((ch == '/' || ch == '*') && potentially_comment) {
      potentially_comment = false;
      single_line_comment = (ch == '/');
      multi_line_comment = (ch == '*');
    } else if (ch == '/')
      potentially_comment = true;
    else {
      if (potentially_comment) {
        potentially_comment = false;
        wrtr.put_next_char('/');
      }

      wrtr.put_next_char(ch);
    }
  }
}

inline void ignore_till_comment_end(reader &rdr, writer &wrtr,
                                    bool &single_line_comment,
                                    bool &multi_line_comment) {
  while (!rdr.is_finished() && (single_line_comment || multi_line_comment)) {
    char ch = rdr.get_next_char();
    rdr.increment_ptr();

    if (ch != '\n')
      wrtr.put_next_char(' ');
    else
      wrtr.put_next_char('\n');

    if (ch == '\n' && single_line_comment)
      single_line_comment = false;
    else if (ch == '*' && !rdr.is_finished() && rdr.get_next_char() == '/' &&
             multi_line_comment) {
      rdr.increment_ptr();
      multi_line_comment = false;
    }
  }
}

void preprocessor::preprocess_file(const std::string &input_file,
                                   const std::string &output_file) {
  remove_comments(input_file, TEMP_FILE);
  remove_trailing_spaces(TEMP_FILE, output_file);

  path to_remove = TEMP_FILE;
  try {
    if (!remove(to_remove))
      cerr << "Unable to remove temporary file of preprocessor\n";
  } catch (filesystem_error &e) {
    cerr << "Filesystem error: " << e.what() << endl;
  }
}

void preprocessor::remove_comments(const std::string &input_file,
                                   const std::string &output_file) {
  reader rdr(input_file);
  writer wrtr(output_file);

  bool single_line_comment = false, multi_line_comment = false;
  while (!rdr.is_finished()) {
    skip_till_comment_start(rdr, wrtr, single_line_comment, multi_line_comment);

    ignore_till_comment_end(rdr, wrtr, single_line_comment, multi_line_comment);

    if (multi_line_comment)
      std::cerr << "Unterminated multi line comment\n";
  }

  rdr.close();
  wrtr.close();
}

void preprocessor::remove_trailing_spaces(const std::string &input_file,
                                          const std::string &output_file) {
  reader rdr(input_file);
  writer wrtr(output_file);

  if (rdr.is_finished())
    return;

  std::queue<char> whitespaces;
  while (!rdr.is_finished()) {
    char ch = rdr.get_next_char();
    rdr.increment_ptr();

    if (ch != '\n' && isspace(ch)) {
      whitespaces.push(ch);
      continue;
    }

    if (ch != '\n') {
      while (!whitespaces.empty()) {
        wrtr.put_next_char(whitespaces.front());
        whitespaces.pop();
      }
    } else {
      while (!whitespaces.empty())
        whitespaces.pop();
    }
    wrtr.put_next_char(ch);
  }

  rdr.close();
  wrtr.close();
}
