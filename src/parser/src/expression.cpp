#include "expression.hpp"
#include <variant>

template<typename T>
T LiteralExpression<T>::evaluate() {
    return literal_.value();
}


/*
template<typename A, typename B>
std::variant<bool, A, B> operate(const A& a, const B& b, Token operator) {
  switch (operator.getType()) {
    case TokenType::OPERATOR_PLUS:
      return a + b;
    case TokenType::OPERATOR_MINUS:
      return a - b;
    case TokenType::OPERATOR_MUL:
      return a * b;
    case TokenType::OPERATOR_DIV:
      if (b!= 0) return a / b;
      else throw std::invalid_argument("Division by zero is not allowed");
    default:
      throw std::invalid_argument("Invalid binary operator");
  }
  return false;
}
*/

template<typename L, typename R>
std::variant<bool, L, R> BinaryExpression<L, R>::evaluate() { // TODO: heavy rework required
  L left_value = left_->evaluate();
  L right_value = right_->evaluate();

  return left_value + right_value;
}