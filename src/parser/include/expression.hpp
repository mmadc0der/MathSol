#pragma once

#include "literal.hpp"
#include "../../lexer/include/token.hpp"
#include <string>

// Expression Interface
template<typename T>
class IExpression {
  public:
    T virtual evaluate() const = 0;
};

template<typename T, typename L, typename R>
class BinaryExpression : public IExpression<T> {
  public:
    BinaryExpression(Literal<T>& left, Literal<T>& right, Token& operator_) : left_(left), right_(right), operator_(operator_) {};
    T evaluate() override;

  private:
    Literal<L> left_;
    Literal<R> right_;
    Token operator_;
};
