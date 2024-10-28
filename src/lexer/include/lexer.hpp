#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include "token.hpp"

class Lexer {
public:
  Lexer(const std::string& sourceCode);
  std::vector<Token> tokenize();

private:
  std::string sourceCode_;
};

#endif  // LEXER_HPP