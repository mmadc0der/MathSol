#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>

enum class TokenType {
  // Ключевые слова
  KEYWORD_IF,
  KEYWORD_ELSE,
  KEYWORD_WHILE,
  KEYWORD_FOR,
  KEYWORD_FUNC,
  KEYWORD_RETURN,
  KEYWORD_TRUE,
  KEYWORD_FALSE,
  KEYWORD_NULL,
  KEYWORD_INF,

  // Операторы
  OPERATOR_PLUS,        // +
  OPERATOR_MINUS,       // -
  OPERATOR_MUL,         // *
  OPERATOR_DIV,         // /
  OPERATOR_MOD,         // %
  OPERATOR_POW,         // **
  OPERATOR_INC,         // ++
  OPERATOR_DEC,         // --
  OPERATOR_PLUS_EQ,     // +=
  OPERATOR_MINUS_EQ,    // -=
  OPERATOR_MUL_EQ,      // *=
  OPERATOR_DIV_EQ,      // /=

  OPERATOR_ASSIGN,      // =
  OPERATOR_EQ,          // ==
  OPERATOR_NE,          // !=
  OPERATOR_LT,          // <
  OPERATOR_LE,          // <=
  OPERATOR_GT,          // >
  OPERATOR_GE,          // >=
  OPERATOR_AND,         // && or 'and'
  OPERATOR_OR,          // || or 'or'
  OPERATOR_NOT,         // !  or 'not'
  
  // Разделители
  DELIMITER_LBRACKET,   // (
  DELIMITER_RBRACKET,   // (
  DELIMITER_LSQBRACKET, // [
  DELIMITER_RSQBRACKET, // ]
  DELIMITER_LCBRACKET,  // {
  DELIMITER_RCBRACKET,  // }
  DELIMITER_SEMICOLON,  // ;
  DELIMITER_COLON,      // :
  DELIMITER_COMMA,      // ,
  DELIMITER_DOT,        // .
  DELIMITER_DDOT,       // ..
  DELIMITER_ELLIPSIS,   // ...
  DELIMITER_QUESTION,   // ?
  DELIMETER_COMMENT,    // #

  // Константы
  CONSTANT_NUM,         // 'number'
  CONSTANT_STRING,      // 'string'

  // Идентификаторы
  IDENTIFIER,

  // Конец файла
  END_OF_FILE,

  // Ошибка
  ERROR
};

const std::string nameTT [] = {
  // Ключевые слова
  "KEYWORD_IF",
  "KEYWORD_ELSE",
  "KEYWORD_WHILE",
  "KEYWORD_FOR",
  "KEYWORD_FUNC",
  "KEYWORD_RETURN",
  "KEYWORD_TRUE",
  "KEYWORD_FALSE",
  "KEYWORD_NULL",
  "KEYWORD_INF",

  // Операторы
  "OPERATOR_PLUS",        // +
  "OPERATOR_MINUS",       // -
  "OPERATOR_MUL",         // *
  "OPERATOR_DIV",         // /
  "OPERATOR_MOD",         // %
  "OPERATOR_POW",         // **
  "OPERATOR_INC",         // ++
  "OPERATOR_DEC",         // --
  "OPERATOR_PLUS_EQ",     // +=
  "OPERATOR_MINUS_EQ",    // -=
  "OPERATOR_MUL_EQ",      // *=
  "OPERATOR_DIV_EQ",      // /=

  "OPERATOR_ASSIGN",      // =
  "OPERATOR_EQ",          // ==
  "OPERATOR_NE",          // !=
  "OPERATOR_LT",          // <
  "OPERATOR_LE",          // <=
  "OPERATOR_GT",          // >
  "OPERATOR_GE",          // >=
  "OPERATOR_AND",         // && or 'and'
  "OPERATOR_OR",          // || or 'or'
  "OPERATOR_NOT",         // !  or 'not'
  
  // Разделители
  "DELIMITER_LBRACKET",   // (
  "DELIMITER_RBRACKET",   // (
  "DELIMITER_LSQBRACKET", // [
  "DELIMITER_RSQBRACKET", // ]
  "DELIMITER_LCBRACKET",  // {
  "DELIMITER_RCBRACKET",  // }
  "DELIMITER_SEMICOLON",  // ;
  "DELIMITER_COLON",      // :
  "DELIMITER_COMMA",      // ,
  "DELIMITER_DOT",        // .
  "DELIMITER_DDOT",       // ..
  "DELIMITER_ELLIPSIS",   // ...
  "DELIMITER_QUESTION",   // ?
  "DELIMETER_COMMENT",    // #

  // Константы
  "CONSTANT_NUM",         // 'number'
  "CONSTANT_STRING",      // 'string'

  // Идентификаторы
  "IDENTIFIER",

  // Конец файла
  "END_OF_FILE",

  // Ошибка
  "ERROR"
};

class Token {
public:
  Token(TokenType type, std::string value)
    : type_(type), value_(value) {}

  TokenType getType() const { return type_; }
  std::string getValue() const { return value_; }
  std::ostream& operator << (std::ostream& outs) { return outs << "[" << nameTT[static_cast<int>(type_)] << ": " << value_ << "]"; }

private:
  TokenType type_;
  std::string value_;
};


#endif  // TOKEN_HPP