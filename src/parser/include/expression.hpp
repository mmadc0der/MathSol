#pragma once

#include "literal.hpp"
#include "../../lexer/include/token.hpp"
#include <string>
#include <variant>

// Expression Interface
template<typename T>
class IExpression {
  public:
    T virtual evaluate() const = 0;
};

template<typename T>
class LiteralExpression : public IExpression {
  public:
    LiteralExpression(Literal<T>& literal_) : literal_(literal_) {};
    T evaluate() const override;
  
  private:
    Literal<T> literal_;
};

template<typename T, typename L, typename R>
class BinaryExpression : public IExpression<T> {
  public:
    BinaryExpression(Literal<T>* left, Literal<T>* right, const Token& operator_) : left_(left), right_(right), operator_(operator_) {};
    std::variant<T, L, R> evaluate() const override;

  private:
    IExpression<L>* left_;
    IExpression<L>* right_;
    Token operator_;
};
