#pragma once
#include <string>

// Forward declarations of AST node types
// Expressions
class NumericLiteral;
class StringLiteral;
class BooleanLiteral;
class IdentifierExpression;
class BinaryExpression;
class UnaryExpression;

// Statements
class ExpressionStatement;
// Add other statement types as they appear

class AstVisitor {
public:
    virtual ~AstVisitor() = default;

    // Visit methods for Expression nodes
    virtual std::string visitNumericLiteral(const NumericLiteral& expr) = 0;
    virtual std::string visitStringLiteral(const StringLiteral& expr) = 0;
    virtual std::string visitBooleanLiteral(const BooleanLiteral& expr) = 0;
    virtual std::string visitIdentifierExpression(const IdentifierExpression& expr) = 0;
    virtual std::string visitBinaryExpression(const BinaryExpression& expr) = 0;
    virtual std::string visitUnaryExpression(const UnaryExpression& expr) = 0;

    // Visit methods for Statement nodes
    virtual std::string visitExpressionStatement(const ExpressionStatement& stmt) = 0;
    // Add other visit methods for statements as they appear
};
