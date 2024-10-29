#include "lexer.hpp"
#include <set>

Lexer::Lexer() : tokens_() {}

Token Lexer::scan_next_(const std::string& sourceCode, int& iter) {
  int last_index = sourceCode.size() - 1;
  int local_index = 0;
  std::set<int> valid_tokens;
  std::string value = "";

  if (iter > last_index) return Token(TokenType::_EOF, "EOF");
  while (sourceCode[iter] == ' ' || sourceCode[iter] == '\t') iter++;
  value += sourceCode[iter];

  for (int i = 0; i < static_cast<int>(TokenType::ERROR); i++) {
    if (valueTT[i][local_index] == value[local_index]) valid_tokens.insert(i);
  }

  iter++;
  local_index++;
  while (iter <= last_index) {
    if (sourceCode[iter] == ' ' || sourceCode[iter] == '\t') break;
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

    if (valid_tokens.size() == 1) {
      iter++;
      std::string target_value = valueTT[*(valid_tokens.begin())];
      Token current_token = scan_const_or_id_(sourceCode, iter, value);
      if (target_value == current_token.getValue()) return Token(static_cast<TokenType>(*(valid_tokens.begin())), value);
      else return current_token;
    }
    iter++; local_index++;
  }

  if (valid_tokens.size() >= 1) {
    for (int i : valid_tokens) {
      if (valueTT[i] == value) return Token(static_cast<TokenType>(i), value);
    }
  }
  return scan_const_or_id_(sourceCode, iter, value);
}


Token Lexer::scan_const_or_id_(const std::string& sourceCode, int& iter, std::string& value) {
  TokenType tt = TokenType::ERROR;
  if (value[0] == '#') {                                      // DELIMETER_COMMENT
    tt = TokenType::DELIMETER_COMMENT;
    while (iter < sourceCode.size()) {
      if (sourceCode[iter] != '\n') value += sourceCode[iter];
      else return Token(TokenType::DELIMETER_COMMENT, value);
      iter++;
    }
  } else if (value[0] >= '0' && value[0] <= '9') {            // CONSTANT_NUM
    tt = TokenType::CONSTANT_NUM;
    bool doted = false;
    while (iter < sourceCode.size()) {
      if (sourceCode[iter] >= '0' && sourceCode[iter] <= '9') value += sourceCode[iter];
      else if (sourceCode[iter] == '.' && !doted) {
        value += sourceCode[iter];
        doted = true;
      }
      else return Token(TokenType::CONSTANT_NUM, value);
      iter++;
    }
  } else if (value[0] == '\'' || value[0] == '\"') {         // CONSTANT_STRING
    tt = TokenType::CONSTANT_STRING;
    char opener = value[0];
    while (iter <= sourceCode.size()) {
      if (sourceCode[iter] != opener) value += sourceCode[iter];
      else {
        value += sourceCode[iter];
        iter++;
        return Token(TokenType::CONSTANT_STRING, value);
      }
      iter++;
    }
  } else if (value[0] == '_'                                  // IDENTIFIER
    || (value[0] >= 'A' && value[0] <= 'Z') 
    || (value[0] >= 'a' && value[0] <= 'z')) {
    while (iter <= sourceCode.size()) {
      tt = TokenType::IDENTIFIER;
      if (sourceCode[iter] == '-' 
        || sourceCode[iter] == '_' 
        || (sourceCode[iter] >= 'A' && sourceCode[iter] <= 'Z') 
        || (sourceCode[iter] >= 'a' && sourceCode[iter] <= 'z')
        || (sourceCode[iter] >= '0' && sourceCode[iter] <= '9')) 
      value += sourceCode[iter];
      else return Token(TokenType::IDENTIFIER, value);
      iter++;
    }
  }
  return Token(tt, value);  
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