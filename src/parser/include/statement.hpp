#pragma once

#include "../../lexer/include/token.hpp"
#include "expression.hpp"
#include <string>

class IStatement {
  public:
    virtual void execute() const = 0;
};

template<typename T>
class ExpressionStatement : public IStatement {
  public:
    ExpressionStatement(IExpression<T>& expression_) : expression_(expression_) {};
    void execute() const override;

  private:
    IExpression<T> expression_;
};