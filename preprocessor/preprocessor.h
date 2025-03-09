#ifndef PREPROCESSOR_H

#define PREPROCESSOR_H

#include <string>

namespace preprocessor {
const std::string TEMP_FILE = ".temp.i";
void preprocess_file(const std::string &input_file,
                     const std::string &output_file);
void remove_comments(const std::string &input_file,
                     const std::string &output_file);
void remove_trailing_spaces(const std::string &input_file,
                            const std::string &output_file);
} // namespace preprocessor
#endif /* PREPROCESSOR_H */
