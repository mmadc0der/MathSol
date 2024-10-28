#include "lexer.hpp"
#include <set>

Lexer::Lexer() : tokens_() {}

Token Lexer::scan_next_(const std::string& sourceCode, int& iter) {
  int last_index = sourceCode.size() - 1;
  int local_index = 0;
  std::set<int> valid_tokens;
  std::string value = "";

  if (iter > last_index) return Token(TokenType::_EOF, "EOF");
  while (sourceCode[iter] == ' ') iter++;
  value += sourceCode[iter];

  for (int i = 0; i < static_cast<int>(TokenType::ERROR); i++) {
    if (valueTT[i][local_index] == value[local_index]) valid_tokens.insert(i);
  }

  iter++;
  local_index++;
  while (iter <= last_index) {
    value += sourceCode[iter];

    std::set<int> tokens_to_erase;
    for (int i : valid_tokens) {
      if (valueTT[i].size() <= local_index) tokens_to_erase.insert(i);
      else if (valueTT[i][local_index] != value[local_index]) tokens_to_erase.insert(i);
      else continue;
    }
    if (valid_tokens.size() == tokens_to_erase.size()) { 
      value.pop_back();
      break; 
    }
    for (int t : tokens_to_erase) valid_tokens.erase(t);

    if (valid_tokens.size() > 1) continue;  
    else {
      iter++;
      return Token(static_cast<TokenType>(*(valid_tokens.begin())), value);
    }
  }

  if (valid_tokens.size() >= 1) {
    for (int i : valid_tokens) {
      if (valueTT[i] == value) return Token(static_cast<TokenType>(i), value);
    }
  }
  return scan_const_(sourceCode, iter, value);
}


Token Lexer::scan_const_(const std::string& sourceCode, int& iter, std::string& value) {  
  TokenType type = TokenType::CONSTANT_NUM;
  for (char c : value) if (c < '0' || c > '9') {
    type = TokenType::CONSTANT_STRING;
    break;
  }
  
  while (iter <= sourceCode.size()) {
    if (type == TokenType::CONSTANT_NUM) {
      if (sourceCode[iter] >= '0' && sourceCode[iter] <= '9') value += sourceCode[iter];
      else return Token(TokenType::CONSTANT_NUM, value);
    } else {
      if (sourceCode[iter] == '-' 
        || sourceCode[iter] == '_' 
        || (sourceCode[iter] >= 'A' && sourceCode[iter] <= 'Z') 
        || (sourceCode[iter] >= 'a' && sourceCode[iter] <= 'z')
        || (sourceCode[iter] >= '0' && sourceCode[iter] <= '9')) 
      value += sourceCode[iter];
      else return Token(TokenType::CONSTANT_STRING, value);
    }
    iter++;
  }
  return Token(type, value);  
}


std::vector<Token> Lexer::tokenize(const std::string& sourceCode) {
  std::vector<Token> tokens;  
  int iter = 0;
  
  Token current = Lexer::scan_next_(sourceCode, iter);
  while (current != Token(TokenType::ERROR) && current != Token(TokenType::_EOF) && current != Token(TokenType::EOL)) {
    tokens.push_back(current);
    current = Lexer::scan_next_(sourceCode, iter);
  }
  tokens.push_back(current);

  Lexer::tokens_.insert(Lexer::tokens_.end(), tokens.begin(), tokens.end());
  return tokens;
}