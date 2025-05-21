#pragma once

#include "ast_visitor.hpp"
#include "expression.hpp" 
#include "statement.hpp"  
#include <string>
#include <vector> // Для std::vector<std::unique_ptr<IStatement>>
#include <memory>   // Для std::unique_ptr
#include <sstream> 
#include <initializer_list> // Для std::initializer_list

// Forward declarations для expression.hpp и statement.hpp, чтобы избежать прямого включения в заголовок,
// если это возможно и достаточно. Однако, для доступа к членам (например, expr.left_)
// нам могут понадобиться полные определения.
// class IExpression;
// class NumericLiteral;
// class StringLiteral;
// class BooleanLiteral;
// class IdentifierExpression;
// class BinaryExpression;
// class UnaryExpression;
// class IStatement;
// class ExpressionStatement;

class AstPrinter : public AstVisitor {
public:
    std::string print(const IExpression& expression);
    std::string print(const IStatement& statement);
    std::string print(const std::vector<std::unique_ptr<IStatement>>& statements);

    // Visit methods for Expression nodes
    std::string visitNumericLiteral(const NumericLiteral& expr) override;
    std::string visitStringLiteral(const StringLiteral& expr) override;
    std::string visitBooleanLiteral(const BooleanLiteral& expr) override;
    std::string visitIdentifierExpression(const IdentifierExpression& expr) override;
    std::string visitBinaryExpression(const BinaryExpression& expr) override;
    std::string visitUnaryExpression(const UnaryExpression& expr) override;

    // Visit methods for Statement nodes
    std::string visitExpressionStatement(const ExpressionStatement& stmt) override;

private:
    // Вспомогательная функция для создания строки с отступом и скобками для родительских узлов
    std::string parenthesize(const std::string& name, int currentIndent, const std::vector<const IExpression*>& expressions);
    std::string parenthesize(const std::string& name, int currentIndent, std::initializer_list<const IExpression*> expressions_list); // Удобная обертка
    
    // Вспомогательная функция для генерации отступов
    std::string indent(int level) const;
    
    int m_currentIndentLevel = 0; // Для управления отступами при печати
}; 
