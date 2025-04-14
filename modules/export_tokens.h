#ifndef MODULES_EXPORT_TOKENS_H

#define MODULES_EXPORT_TOKENS_H

#include <cstddef>
#include <modules/token.h>

std::string get_keyword_as_string(size_t kw);
std::string get_operator_as_string(size_t op);
std::string get_token_as_string(const ycc::token &tok);

#endif /* MODULES_EXPORT_TOKENS_H */
