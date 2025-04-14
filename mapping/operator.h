#ifndef MAPPING_OPERATOR_H

#define MAPPING_OPERATOR_H

#include <string>
#include <unordered_map>

namespace ycc {
enum operators_mapping {
  L_SQUARE,
  R_SQUARE,
  L_BRACE,
  R_BRACE,
  L_PAREN,
  R_PAREN,
  COLONCOLON,
  COLONEQUAL,
  LESSLESS,
  LESSEQUAL,
  LESSGREATER,
  GREATERGREATER,
  GREATEREQUAL,
  EQUALEQUAL,
  PLUSPLUS,
  PLUSEQUAL,
  MINUS,
  PERCENT,
  STAR,
  SLASH,
  PIPEPIPE,
  AMPAMP,
  EXCLAIMEQUAL,
  LESS,
  GREATER,
};
const std::unordered_map<std::string, operators_mapping> OPERATORS_MAP = {
    {"l_square", operators_mapping::L_SQUARE},
    {"r_square", operators_mapping::R_SQUARE},
    {"l_brace", operators_mapping::L_BRACE},
    {"r_brace", operators_mapping::R_BRACE},
    {"l_paren", operators_mapping::L_PAREN},
    {"r_paren", operators_mapping::R_PAREN},
    {"coloncolon", operators_mapping::COLONCOLON},
    {"colonequal", operators_mapping::COLONEQUAL},
    {"lessless", operators_mapping::LESSLESS},
    {"lessequal", operators_mapping::LESSEQUAL},
    {"lessgreater", operators_mapping::LESSGREATER},
    {"greatergreater", operators_mapping::GREATERGREATER},
    {"greaterequal", operators_mapping::GREATEREQUAL},
    {"equalequal", operators_mapping::EQUALEQUAL},
    {"plusplus", operators_mapping::PLUSPLUS},
    {"plusequal", operators_mapping::PLUSEQUAL},
    {"minus", operators_mapping::MINUS},
    {"percent", operators_mapping::PERCENT},
    {"star", operators_mapping::STAR},
    {"slash", operators_mapping::SLASH},
    {"pipepipe", operators_mapping::PIPEPIPE},
    {"ampamp", operators_mapping::AMPAMP},
    {"exclaimequal", operators_mapping::EXCLAIMEQUAL},
    {"less", operators_mapping::LESS},
    {"greater", operators_mapping::GREATER},
};
} // namespace ycc
#endif /* MAPPING_OPERATOR_H */
