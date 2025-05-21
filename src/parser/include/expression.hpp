#pragma once

#include "../../lexer/include/token.hpp"
#include <string>
#include <variant>
#include <memory> // Для std::unique_ptr
#include "../../lexer/include/token.hpp" // Для Token
#include "ast_visitor.hpp" // Для AstVisitor

// Определяем возможные типы значений, которые могут возвращать выражения
using Value = std::variant<double, bool, std::string>; 

class Environment; // Forward declaration

// Базовый интерфейс для всех узлов выражений AST
class IExpression {
public:
    virtual ~IExpression() = default;
    virtual Value evaluate(Environment& env) const = 0;
    virtual std::string accept(AstVisitor& visitor) const = 0;
};

// Конкретные классы выражений

// Для числовых литералов (например, 123, 45.67)
class NumericLiteral : public IExpression {
public:
    double value_;
    explicit NumericLiteral(double val);
    Value evaluate(Environment& env) const override;
    std::string accept(AstVisitor& visitor) const override;
};

// Для строковых литералов (например, "hello")
class StringLiteral : public IExpression {
public:
    std::string value_;
    explicit StringLiteral(std::string val);
    Value evaluate(Environment& env) const override;
    std::string accept(AstVisitor& visitor) const override;
};

// Для булевых литералов (например, true, false)
class BooleanLiteral : public IExpression {
public:
    bool value_;
    explicit BooleanLiteral(bool val);
    Value evaluate(Environment& env) const override;
    std::string accept(AstVisitor& visitor) const override;
};

// Для идентификаторов (переменных)
class IdentifierExpression : public IExpression {
public:
    Token name_token_;
    explicit IdentifierExpression(Token token);
    virtual std::string getName() const { return name_token_.getValue(); }
    Value evaluate(Environment& env) const override;
    std::string accept(AstVisitor& visitor) const override;
};

// Для бинарных выражений (например, a + b, 1 * 2)
class BinaryExpression : public IExpression {
public:
    std::unique_ptr<IExpression> left_;
    Token operator_token_; // Токен оператора (например, +, -, *, /)
    std::unique_ptr<IExpression> right_;

    BinaryExpression(std::unique_ptr<IExpression> left, 
                     Token op_token, 
                     std::unique_ptr<IExpression> right);
    Value evaluate(Environment& env) const override;
    std::string accept(AstVisitor& visitor) const override;
};

// --- UnaryExpression ---
// Представляет унарную операцию (например, !, -)
class UnaryExpression : public IExpression {
public:
    Token operator_token_; // Токен оператора (например, "!" или "-")
    std::unique_ptr<IExpression> right_; // Операнд

    UnaryExpression(Token op_token, std::unique_ptr<IExpression> right);
    Value evaluate(Environment& env) const override;
    std::string accept(AstVisitor& visitor) const override;
};

// TODO: Позже сюда добавим BinaryExpression, CallExpression и др.
// Они будут использовать std::unique_ptr<IExpression> для дочерних узлов.
// Например:
/*
class BinaryExpression : public IExpression {
public:
    std::unique_ptr<IExpression> left_;
    Token operator_;
    std::unique_ptr<IExpression> right_;

    BinaryExpression(std::unique_ptr<IExpression> left, 
                     Token op, 
                     std::unique_ptr<IExpression> right)
        : left_(std::move(left)), operator_(std::move(op)), right_(std::move(right)) {}

    Value evaluate(Environment& env) const override {
        Value left_val = left_->evaluate(env);
        Value right_val = right_->evaluate(env);
        // ... логика бинарной операции в зависимости от operator_ и типов left_val/right_val ...
        // Например, для сложения:
        // if (std::holds_alternative<double>(left_val) && std::holds_alternative<double>(right_val)) {
        //     return std::get<double>(left_val) + std::get<double>(right_val);
        // }
        // ... обработка ошибок типов ...
        // throw std::runtime_error("Type error in binary expression.");
        return {}; // Placeholder
    }
};

*/
