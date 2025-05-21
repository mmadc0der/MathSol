#include "../include/expression.hpp"
#include <stdexcept>

// --- NumericLiteral ---
std::string NumericLiteral::accept(AstVisitor& visitor) const {
    return visitor.visitNumericLiteral(*this);
}

NumericLiteral::NumericLiteral(double val) : value_(val) {}

Value NumericLiteral::evaluate(Environment& env) const {
    (void)env; 
    return value_;
}

// --- StringLiteral ---
std::string StringLiteral::accept(AstVisitor& visitor) const {
    return visitor.visitStringLiteral(*this);
}

StringLiteral::StringLiteral(std::string val) : value_(std::move(val)) {}

Value StringLiteral::evaluate(Environment& env) const {
    (void)env; 
    return value_;
}

// --- BooleanLiteral ---
std::string BooleanLiteral::accept(AstVisitor& visitor) const {
    return visitor.visitBooleanLiteral(*this);
}

BooleanLiteral::BooleanLiteral(bool val) : value_(val) {}

Value BooleanLiteral::evaluate(Environment& env) const {
    (void)env; 
    return value_;
}

// --- IdentifierExpression ---
std::string IdentifierExpression::accept(AstVisitor& visitor) const {
    return visitor.visitIdentifierExpression(*this);
}

IdentifierExpression::IdentifierExpression(Token token) : name_token_(std::move(token)) {}


Value IdentifierExpression::evaluate(Environment& env) const {
    (void)env; 
    // В реальной реализации здесь будет что-то вроде: return env.get(name_token_.getValue());
    // Пока что, чтобы код компилировался, вернем какое-нибудь значение по умолчанию или бросим исключение.
    // Эта заглушка должна быть заменена реальной логикой или четким сообщением об ошибке.
    // throw std::runtime_error("Environment not implemented for IdentifierExpression: " + getName());
    return "Identifier_evaluated_placeholder"; 
}

// --- BinaryExpression ---
std::string BinaryExpression::accept(AstVisitor& visitor) const {
    return visitor.visitBinaryExpression(*this);
}

BinaryExpression::BinaryExpression(std::unique_ptr<IExpression> left, 
                                 Token op_token, 
                                 std::unique_ptr<IExpression> right)
    : left_(std::move(left)), 
      operator_token_(std::move(op_token)), 
      right_(std::move(right)) {}

Value BinaryExpression::evaluate(Environment& env) const {
    Value left_val = left_->evaluate(env);
    Value right_val = right_->evaluate(env);

    if (operator_token_.getType() == TokenType::OPERATOR_PLUS) { 
        if (std::holds_alternative<double>(left_val) && std::holds_alternative<double>(right_val)) {
            return std::get<double>(left_val) + std::get<double>(right_val);
        }
        // TODO: Добавить обработку других типов (например, конкатенация строк, если поддерживается)
        // TODO: Добавить обработку ошибок типов (например, если пытаемся сложить число и строку без явного приведения)
        // throw std::runtime_error("Runtime Error: Incomplete binary expression evaluation logic.");
    }
    // TODO: Добавить логику для других операторов (MINUS, STAR, SLASH, <, >, ==, etc.)
    //  - Для каждого оператора нужно будет проверить типы операндов (left_val, right_val)
    //  - Выполнить операцию
    //  - Вернуть результат типа Value
    //  - Обработать возможные ошибки (деление на ноль, несоответствие типов)

    // Временная заглушка / сообщение об ошибке, если оператор не обработан
    // throw std::runtime_error("Binary operation not (fully) implemented for operator: " + operator_token_.getValue());
    return "Binary_expression_placeholder"; 
}

// --- UnaryExpression ---
std::string UnaryExpression::accept(AstVisitor& visitor) const {
    return visitor.visitUnaryExpression(*this);
}

UnaryExpression::UnaryExpression(Token op_token, std::unique_ptr<IExpression> right)
    : operator_token_(std::move(op_token)), right_(std::move(right)) {}

Value UnaryExpression::evaluate(Environment& env) const {
    Value right_val = right_->evaluate(env);

    switch (operator_token_.getType()) {
        case TokenType::OPERATOR_NOT:
            if (std::holds_alternative<bool>(right_val)) {
                return !std::get<bool>(right_val);
            }
            // TODO: Добавить номер строки в сообщение об ошибке, если Token::getLine() существует
            throw std::runtime_error("Runtime Error: Operand for '!' must be a boolean.");
        case TokenType::OPERATOR_MINUS:
            if (std::holds_alternative<double>(right_val)) {
                return -std::get<double>(right_val);
            }
            // TODO: Добавить номер строки в сообщение об ошибке, если Token::getLine() существует
            throw std::runtime_error("Runtime Error: Operand for unary '-' must be a number.");
        default:
            // TODO: Добавить номер строки в сообщение об ошибке, если Token::getLine() существует
            throw std::runtime_error("Runtime Error: Unknown unary operator '" + operator_token_.getValue() + "'.");
    }
}