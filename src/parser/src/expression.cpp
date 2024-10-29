#include "expression.hpp"

template<typename T>
T LiteralExpression<T>::evaluate() const {
    return literal_.value();
}

template<typename T, typename L, typename R>
std::variant<T, L, R> BinaryExpression<T, L, R>::evaluate() const { // TODO: heavy rework required
  std::variant<L, R> left_value = left_->evaluate();
  std::variant<L, R> right_value = right_->evaluate();

  std::variant<T, L, R> result = nullptr;

  switch (token_.getType()) {
    case TokenType::OPERATOR_PLUS:
      result = left_value + right_value;
      break;
    case TokenType::OPERATOR_MINUS:
      result = left_value - right_value;
      break;
    case TokenType::OPERATOR_MUL: {
      if (std::is_same_v(L, double)) result = left_value * right_value;
      else throw std::invalid_argument("Strings multipllication is not permited");
      break;
    }
    case TokenType::OPERATOR_DIV: {
      if (std::is_same_v(L, double)) {
        if (right_value != 0) result = left_value / right_value;
        else throw std::invalid_argument("Division by zero is not allowed");
      } else throw std::invalid_argument("Strings division is not permited");
      break;
    }
    case TokenType::OPERATOR_EQ:
      result = left_value == right_value;
      break;
    case TokenType::OPERATOR_NE:
      result = left_value != right_value;
      break;
    default:
      throw std::invalid_argument("Invalid binary operator");
  }

  return result;
}