#ifndef PREPROCESSOR_H

#define PREPROCESSOR_H

#include <string>

namespace preprocessor {
void preprocess_file(const std::string &input_file,
                     const std::string &output_file);
} // namespace preprocessor
#endif /* PREPROCESSOR_H */
