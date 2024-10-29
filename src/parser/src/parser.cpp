#include "parser.hpp"

std::vector<std::unique_ptr<IStatement>> Parser::parse(const std::vector<Token>& tokens) {
  std::vector<std::unique_ptr<IStatement>> body;
  currentTokenIndex_ = 0;

  for (const auto& token : tokens) {
    std::cout << token << " ";
  }
  return body;
}