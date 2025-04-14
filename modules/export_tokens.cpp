#include <iostream>
#include <mapping/keyword.h>
#include <mapping/operator.h>
#include <modules/export_tokens.h>
#include <modules/token.h>
#include <sstream>

using namespace ycc;

std::string get_keyword_as_string(size_t kw) {
  switch (kw) {
  case ASM:
    return "asm";
  case WAGARNA:
    return "wagarna";
  case NEW:
    return "new";
  case THIS:
    return "this";
  case AUTO:
    return "auto";
  case ENUM:
    return "enum";
  case KWOPERATOR:
    return "kwoperator";
  case THROW:
    return "throw";
  case MANTIQI:
    return "mantaqi";
  case EXPLICIT:
    return "explicit";
  case PRIVATE:
    return "private";
  case TRUE:
    return "true";
  case BREAK:
    return "break";
  case EXPORT:
    return "export";
  case PROTECTED:
    return "protected";
  case TRY:
    return "try";
  case CASE:
    return "case";
  case EXTERN:
    return "extern";
  case PUBLIC:
    return "public";
  case TYPEDEF:
    return "typedef";
  case CATCH:
    return "catch";
  case FALSE:
    return "false";
  case REGISTER:
    return "register";
  case TYPEID:
    return "typeid";
  case HARF:
    return "harf";
  case ASHRIYA:
    return "ashriya";
  case TYPENAME:
    return "typename";
  case ADADI:
    return "adadi";
  case CLASS:
    return "class";
  case FOR:
    return "for";
  case WAPAS:
    return "wapas";
  case UNION:
    return "union";
  case CONST:
    return "const";
  case DOST:
    return "dost";
  case SHORT:
    return "short";
  case UNSINGED:
    return "unsinged";
  case GOTO:
    return "goto";
  case SIGNED:
    return "signed";
  case USING:
    return "using";
  case CONTINUE:
    return "continue";
  case AGAR:
    return "agar";
  case SIZEOF:
    return "sizeof";
  case VIRTUAL:
    return "virtual";
  case DEFAULT:
    return "default";
  case INLINE:
    return "inline";
  case STATIC:
    return "static";
  case KHALI:
    return "khali";
  case DELETE:
    return "delete";
  case VOLATILE:
    return "volatile";
  case DO:
    return "do";
  case LONG:
    return "long";
  case STRUCT:
    return "struct";
  case DOUBLE:
    return "double";
  case MUTABLE:
    return "mutable";
  case SWITCH:
    return "switch";
  case WHILE:
    return "while";
  case NAMESPACE:
    return "namespace";
  case TEMPLATE:
    return "template";
  case MARQAZI:
    return "marqazi";
  case MATN:
    return "matn";
  case INPUT:
    return "input";
  case OUTPUT:
    return "output";
  default:
    return "";
  }
}

std::string get_operator_as_string(size_t op) {
  switch (op) {
  case L_SQUARE:
    return "[";
  case R_SQUARE:
    return "]";
  case L_BRACE:
    return "{";
  case R_BRACE:
    return "}";
  case L_PAREN:
    return "(";
  case R_PAREN:
    return ")";
  case COLONCOLON:
    return "::";
  case COLONEQUAL:
    return ":=";
  case LESSLESS:
    return "<<";
  case LESSEQUAL:
    return "<=";
  case LESSGREATER:
    return "<>";
  case GREATERGREATER:
    return ">>";
  case GREATEREQUAL:
    return ">=";
  case EQUALEQUAL:
    return "==";
  case PLUSPLUS:
    return "++";
  case PLUSEQUAL:
    return "+=";
  case MINUS:
    return "-";
  case PERCENT:
    return "%";
  case STAR:
    return "*";
  case SLASH:
    return "/";
  case PIPEPIPE:
    return "||";
  case AMPAMP:
    return "&&";
  case EXCLAIMEQUAL:
    return "!=";
  case LESS:
    return "<";
  case GREATER:
    return ">";
  default:
    return "";
  }
}

std::string get_token_as_string(const token &tok) {
  std::ostringstream oss;
  oss << "<";
  switch (tok.type) {
  case token_type::ERROR:
    oss << "error";
    if (tok.value.has_value())
      oss << ", " << tok.value.value();
    break;
  case token_type::IDENTIFIER:
    oss << "id";
    oss << ", " << tok.idx;
    break;
  case token_type::KEYWORD:
    oss << "keyword";
    oss << ", " << get_keyword_as_string(tok.idx);
    break;
  case token_type::OPERATOR:
    oss << "operator";
    oss << ", " << get_operator_as_string(tok.idx);
    break;
  case token_type::LITERAL:
    oss << "literal";
    oss << ", " << tok.idx;
    break;
  }

  oss << ">";

  return oss.str();
}
