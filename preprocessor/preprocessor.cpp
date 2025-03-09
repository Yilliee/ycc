#include "preprocessor.h"
#include "../filemanager/reader.h"
#include "../filemanager/writer.h"
#include <iostream>

using filemanager::reader, filemanager::writer;

inline void skip_till_comment_start(reader &rdr, writer &wrtr,
                                    bool &single_line_comment,
                                    bool &multi_line_comment) {
  bool potentially_comment = false;

  while (!rdr.is_finished() && !(single_line_comment || multi_line_comment)) {
    char ch = rdr.get_next_char();

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

inline void ignore_till_comment_end(reader &rdr, bool &single_line_comment,
                                    bool &multi_line_comment) {
  while (!rdr.is_finished() && (single_line_comment || multi_line_comment)) {
    char ch = rdr.get_next_char();

    if (ch == '\n' && single_line_comment)
      single_line_comment = false;
    else if (ch == '*' && !rdr.is_finished() && rdr.get_next_char() == '/' &&
             multi_line_comment)
      multi_line_comment = false;
  }
}

void preprocessor::preprocess_file(const std::string &input_file,
                                   const std::string &output_file) {
  reader rdr(input_file);
  writer wrtr(output_file);

  bool single_line_comment = false, multi_line_comment = false;
  while (!rdr.is_finished()) {
    skip_till_comment_start(rdr, wrtr, single_line_comment, multi_line_comment);

    ignore_till_comment_end(rdr, single_line_comment, multi_line_comment);

    if (multi_line_comment)
      std::cerr << "Unterminated multi line comment\n";
  }

  rdr.close();
  wrtr.close();
}
