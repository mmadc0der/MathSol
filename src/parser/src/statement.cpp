#include "../include/statement.hpp"

// --- ExpressionStatement ---
std::string ExpressionStatement::accept(AstVisitor& visitor) const {
    return visitor.visitExpressionStatement(*this);
}

// #include "../include/environment.hpp" // Раскомментировать, когда Environment будет готов

ExpressionStatement::ExpressionStatement(std::unique_ptr<IExpression> expr)
    : expression_(std::move(expr)) {}

void ExpressionStatement::execute(Environment& env) const {
    // Просто вычисляем выражение. Результат игнорируется, 
    // но вычисление может иметь побочные эффекты (например, вызов функции).
    expression_->evaluate(env);
}