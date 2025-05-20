#pragma once

#include "literal.hpp"
#include "../../lexer/include/token.hpp"
#include <string>
#include <variant>

// Expression Interface
template<typename T>
class IExpression {
  public:
    T virtual evaluate() = 0;
};

template<typename T>
class LiteralExpression : public IExpression<T> {
  public:
    LiteralExpression(Literal<T>& literal_) : literal_(literal_) {};
    T evaluate() override;
  
  private:
    Literal<T> literal_;
};

template<typename L, typename R>
class BinaryExpression : public IExpression<L> {
  public:
    BinaryExpression(IExpression<L>* left, IExpression<L>* right, const Token& operator_) : left_(left), right_(right), operator_(operator_) {};
    std::variant<bool, L, R> evaluate() override;

  private:
    IExpression<L>* left_;
    IExpression<L>* right_;
    Token operator_;
};
