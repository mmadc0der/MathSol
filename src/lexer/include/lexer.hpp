#pragma once
#include <vector>
#include <string>
#include "token.hpp"

class Lexer {
public:
  Lexer();
  
  // Process source code and return tokens
  std::vector<Token> tokenize(const std::string& sourceCode);
  
  // Get EOF token and finalize tokenization
  std::vector<Token> eof();

private:
  std::string pending_token_value_;   // Raw string value of the last pending token
  
  // Internal token scanning methods
  std::pair<Token, bool> scan_next_(const std::string& sourceCode, size_t& iter);
  std::pair<Token, bool> scan_const_or_id_(const std::string& sourceCode, size_t& iter, std::string& value, bool initial_char_consumed);
};
