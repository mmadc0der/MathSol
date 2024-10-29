#pragma once
#include <vector>
#include "token.hpp"

class Lexer {
public:
  Lexer();
  std::vector<Token> tokenize(const std::string& sourceCode);

private:
  std::vector<Token> tokens_;

  Token scan_next_(const std::string& sourceCode, int& iter);
  Token scan_const_or_id_(const std::string& sourceCode, int& iter, std::string& value);
};
