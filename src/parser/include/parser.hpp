#pragma once

#include "../../lexer/include/token.hpp"
#include "literal.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include <vector>
#include <memory>
#include <variant>

class Parser {
public:
  Parser();

  std::vector<std::unique_ptr<IStatement>> parse(const std::vector<Token>& tokens);

private:
  const std::vector<IStatement*>& body_;
  const std::vector<Token>& tokens_;
  int currentTokenIndex_;

  bool match(TokenType type);
  Token advance();

  bool parseStatement();
  bool parseExpressionStatement();
  bool parseIfStatement();
  bool parseWhileStatement();
  bool parseForStatement();
  bool parseFunctionDeclaration();
  bool parseReturnStatement();
  bool parseBooleanLiteral();
  bool parseIntegerLiteral();
  bool parseIdentifier();
  bool parseUnaryExpression();
  bool parseBinaryExpression(int precedence);
};