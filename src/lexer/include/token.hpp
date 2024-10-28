#pragma once
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
  KEYWORD_AND,          // 'and'
  KEYWORD_OR,           // 'or'
  KEYWORD_NOT,          // 'not'

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
  OPERATOR_AND,         // &&
  OPERATOR_OR,          // ||
  OPERATOR_NOT,         // !
  
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
  _EOF,
  EOL,

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
  "KEYWORD_AND",         // 'and'
  "KEYWORD_OR",          // 'or'
  "KEYWORD_NOT",         // 'not'

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
  "OPERATOR_AND",         // &&
  "OPERATOR_OR",          // ||
  "OPERATOR_NOT",         // !
  
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
  "EOF",
  "EOL",

  // Ошибка
  "ERROR"
};

const std::string valueTT [] = {
  // Ключевые слова
  "if",               // KEYWORD_IF
  "else",             // KEYWORD_ELSE
  "while",            // KEYWORD_WHILE
  "for",              // KEYWORD_FOR
  "func",
  "return",
  "true",
  "false",
  "null",
  "inf",
  "and",
  "or",
  "not",

  // Операторы
  "+",
  "-",
  "*",
  "/",
  "%",
  "**",
  "++",
  "--",
  "+=",
  "-=",
  "*=",
  "/=",

  "=",   
  "==",  
  "!=", 
  "<",  
  "<=", 
  ">",
  ">=",  
  "&&",  
  "||", 
  "!", 
  
  // Разделители
  "(",
  ")",   // )
  "[", // [
  "]", // ]
  "{",  // {
  "}",  // }
  ";",  // ;
  ":",      // :
  ",",      // ,
  ".",        // .
  "..",       // ..
  "...",   // ...
  "?",   // ?
  "#",    // #

  // Константы
  "",      // number
  "",      // string

  // Идентификаторы
  "",   // identifier

  // Конец файла
  "",   // EOF
  "\n",

  // Ошибка
  ""    // ERROR
};


class Token {
public:
  Token(TokenType type)
    : type_(type), value_() {}
    
  Token(TokenType type, std::string value)
    : type_(type), value_(value) {}

  TokenType getType() const { return type_; }
  std::string getValue() const { return value_; }
  
  friend bool operator==(const Token& lhs, const Token& rhs) { return lhs.getType() == rhs.getType(); }
  friend bool operator!=(const Token& lhs, const Token& rhs) { return lhs.getType() != rhs.getType(); }

private:
  TokenType type_;
  std::string value_;
};

std::ostream& operator << (std::ostream& outs, const Token& t);