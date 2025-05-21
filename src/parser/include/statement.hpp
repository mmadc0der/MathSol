#pragma once

#include "../../lexer/include/token.hpp"
#include "expression.hpp" // Для IExpression и Value
#include "ast_visitor.hpp"  // Для AstVisitor

class Environment; // Forward declaration

class IStatement {
  public:
    virtual ~IStatement() = default;
    virtual void execute(Environment& env) const = 0;
    virtual std::string accept(AstVisitor& visitor) const = 0; // Добавлен параметр Environment
};

// Инструкция-выражение (например, вызов функции или операция, используемая как инструкция)
class ExpressionStatement : public IStatement {
public:
    std::unique_ptr<IExpression> expression_;

    explicit ExpressionStatement(std::unique_ptr<IExpression> expr);
    void execute(Environment& env) const override;
    std::string accept(AstVisitor& visitor) const override;
};