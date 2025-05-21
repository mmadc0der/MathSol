// src/parser/src/ast_printer.cpp
#include "../include/ast_printer.hpp"
#include <iomanip> // Для std::fixed, std::setprecision (для double)
#include <cmath>   // Для std::modf, std::abs

// --- Public Print Methods ---
std::string AstPrinter::print(const IExpression& expression) {
    // m_currentIndentLevel сбрасывается или устанавливается здесь, если это корневой вызов для одного выражения
    // Но для простоты, предполагаем, что visit методы сами управляют своими локальными отступами или используют m_currentIndentLevel как базовый
    return expression.accept(*this);
}

std::string AstPrinter::print(const IStatement& statement) {
    // Аналогично print(IExpression&)
    return statement.accept(*this);
}

std::string AstPrinter::print(const std::vector<std::unique_ptr<IStatement>>& statements) {
    std::stringstream out;
    m_currentIndentLevel = 0; // Сброс для списка инструкций верхнего уровня
    for (const auto& stmt_ptr : statements) {
        if (stmt_ptr) {
            // Каждый visit для statement будет использовать m_currentIndentLevel для своего базового отступа
            out << stmt_ptr->accept(*this) << "\n"; 
        } else {
            out << indent(m_currentIndentLevel) << "[<nullptr statement>]\n";
        }
    }
    return out.str();
}

// --- Helper Methods ---
std::string AstPrinter::indent(int level) const {
    if (level < 0) level = 0;
    return std::string(level * 2, ' '); // 2 пробела на уровень отступа
}

std::string AstPrinter::parenthesize(const std::string& name, int currentIndent, const std::vector<const IExpression*>& expressions) {
    std::stringstream out;
    out << indent(currentIndent) << "[" << name;
    int previousIndent = m_currentIndentLevel;
    m_currentIndentLevel = currentIndent; // Устанавливаем для дочерних узлов
    for (const IExpression* expr_ptr : expressions) {
        if (expr_ptr) {
            out << "\n" << expr_ptr->accept(*this); 
        } else {
            out << "\n" << indent(m_currentIndentLevel + 1) << "[<nullptr expression>]";
        }
    }
    m_currentIndentLevel = previousIndent; // Восстанавливаем
    out << "\n" << indent(currentIndent) << "]";
    return out.str();
}

std::string AstPrinter::parenthesize(const std::string& name, int currentIndent, std::initializer_list<const IExpression*> expressions_list) {
    std::vector<const IExpression*> expressions_vec(expressions_list);
    return parenthesize(name, currentIndent, expressions_vec);
}

// --- Visit Methods for Expressions ---
std::string AstPrinter::visitNumericLiteral(const NumericLiteral& expr) {
    // Prepend (Numeric ...) to the output
    std::stringstream ss;
    double val = expr.value_;
    double intpart;
    // Проверяем, является ли число целым (с некоторой точностью для чисел с плавающей запятой)
    if (std::abs(std::modf(val, &intpart)) < 1e-9) { // Если дробная часть очень мала
        ss << static_cast<long long>(val);
    } else {
        ss << std::fixed << std::setprecision(15) << val;
        std::string s_val = ss.str();
        // The rest of the original logic for formatting the number
        s_val.erase(s_val.find_last_not_of('0') + 1, std::string::npos);
        if (!s_val.empty() && s_val.back() == '.') {
            s_val.pop_back();
        }
        return indent(m_currentIndentLevel + 1) + "[Numeric: " + s_val + "]"; // Changed line
        s_val.erase(s_val.find_last_not_of('0') + 1, std::string::npos);
        if (!s_val.empty() && s_val.back() == '.') {
            s_val.pop_back();
        }
        return indent(m_currentIndentLevel + 1) + s_val;
    }
    return indent(m_currentIndentLevel + 1) + "[Numeric: " + ss.str() + "]"; // Changed line for integer part
}

std::string AstPrinter::visitStringLiteral(const StringLiteral& expr) {
    return indent(m_currentIndentLevel + 1) + "[String: \"" + expr.value_ + "\"]";
}

std::string AstPrinter::visitBooleanLiteral(const BooleanLiteral& expr) {
    return indent(m_currentIndentLevel + 1) + "[Boolean: " + (expr.value_ ? "true" : "false") + "]";
}

std::string AstPrinter::visitIdentifierExpression(const IdentifierExpression& expr) {
    return indent(m_currentIndentLevel + 1) + "[Identifier: " + expr.name_token_.getValue() + "]";
}

std::string AstPrinter::visitUnaryExpression(const UnaryExpression& expr) {
    // parenthesize управляет m_currentIndentLevel для своих детей
    return parenthesize("Unary: " + expr.operator_token_.getValue(), m_currentIndentLevel, {expr.right_.get()});
}

std::string AstPrinter::visitBinaryExpression(const BinaryExpression& expr) {
    // parenthesize управляет m_currentIndentLevel для своих детей
    return parenthesize("Binary: " + expr.operator_token_.getValue(), m_currentIndentLevel, {expr.left_.get(), expr.right_.get()});
}

// --- Visit Methods for Statements ---
std::string AstPrinter::visitExpressionStatement(const ExpressionStatement& stmt) {
    std::stringstream out;
    out << indent(m_currentIndentLevel) << "[ExpressionStatement:";
    if (stmt.expression_) {
        int previousIndent = m_currentIndentLevel;
        m_currentIndentLevel++; // Увеличиваем отступ для самого выражения внутри инструкции
        out << "\n" << stmt.expression_->accept(*this);
        m_currentIndentLevel = previousIndent; // Восстанавливаем отступ
        out << "\n" << indent(m_currentIndentLevel) << "]";
    } else {
        out << "\n" << indent(m_currentIndentLevel + 1) << "[<nullptr expression>]";
        out << "\n" << indent(m_currentIndentLevel) << "]";
    }
    return out.str();
}
