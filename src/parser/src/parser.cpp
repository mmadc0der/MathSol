#include "../include/parser.hpp"
#include "../include/expression.hpp" // Для NumericLiteral, StringLiteral, BooleanLiteral, IdentifierExpression, BinaryExpression
#include "../include/statement.hpp"  // Для ExpressionStatement
#include <stdexcept> // Для std::stod исключений
#include <iostream>  // Для временной отладки

// --- Конструктор --- 
Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens), currentTokenIndex_(0) {}

// --- Основной метод парсинга --- 
std::vector<std::unique_ptr<IStatement>> Parser::parse() {
    std::vector<std::unique_ptr<IStatement>> statements;
    while (!isAtEnd()) {
        // Сначала пропускаем все EOL, которые не являются частью синтаксиса (например, пустые строки)
        while (check(TokenType::EOL)) { // Используем while для пропуска нескольких EOL подряд
            advance();
        }

        if (isAtEnd()) { // Могли дойти до конца, пропуская EOL
            break;
        }

        std::unique_ptr<IStatement> stmt = parseDeclaration(); // Используем parseDeclaration как точку входа
        if (stmt) {
            statements.push_back(std::move(stmt));
        } else {
            // Если parseDeclaration вернул nullptr, и это не EOF (EOL уже пропущены),
            // то это может быть неожиданный токен.
            if (!isAtEnd()) { // isAtEnd() также проверяет _EOF
                // Ошибка: неожиданный токен, который не может начать объявление/инструкцию.
                // Можно добавить логирование ошибки или вызов synchronize().
                // std::cerr << "Parser Error: Unexpected token " << peek().toString() << " cannot start a statement." << std::endl;
                // synchronize(); // Попытка восстановления, если реализована
                break; // Пока что прерываем парсинг при первой такой ошибке.
            }
        }
    }
    return statements;
}

// --- Вспомогательные методы для работы с токенами --- 
Token Parser::advance() {
    if (!isAtEnd()) {
        currentTokenIndex_++;
    }
    return previous();
}

Token Parser::peek() const {
    return tokens_[currentTokenIndex_];
}

Token Parser::previous() const {
    if (currentTokenIndex_ == 0) {
        // Не должно происходить при правильном использовании, но для безопасности
        // Можно бросить исключение или вернуть специальный "нулевой" токен
        throw std::out_of_range("previous() called at the beginning of token stream");
    }
    return tokens_[currentTokenIndex_ - 1];
}

bool Parser::isAtEnd() const {
    return currentTokenIndex_ >= tokens_.size() || peek().getType() == TokenType::_EOF;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().getType() == type;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

// --- Методы для разбора инструкций (Statements) --- 
std::unique_ptr<IStatement> Parser::parseDeclaration() {
    // TODO: Реализовать разбор объявлений (например, var, fun)
    // Если не объявление, то это обычная инструкция
    return parseStatement();
}

std::unique_ptr<IStatement> Parser::parseStatement() {
    // TODO: Добавить разбор других типов инструкций (if, while, for, print, return, block)
    // Пока что все инструкции - это инструкции-выражения
    return parseExpressionStatement();
}

std::unique_ptr<IStatement> Parser::parseExpressionStatement() {
    std::unique_ptr<IExpression> expr = parseExpression();
    if (!expr) {
        // Ошибка при разборе выражения, или это не инструкция-выражение
        return nullptr;
    }
    // В некоторых языках точка с запятой обязательна, в других - нет.
    // Для MathSol, предположим, что она опциональна или определяется контекстом (например, REPL).
    // Ожидаем точку с запятой или EOL как терминатор ExpressionStatement.
    // Если их нет, и мы не в конце файла, это может быть ошибкой в более строгом парсере.
    // Пока что сделаем их опциональными, но если они есть - съедим.
    if (match({TokenType::DELIMITER_SEMICOLON, TokenType::EOL})) {
        // Токен был съеден
    } else if (!isAtEnd() && peek().getType() != TokenType::_EOF) {
        // Если после выражения идет что-то, что не является разделителем или EOF,
        // это может быть ошибкой в некоторых грамматиках. 
        // Например, 'print 10 20' - здесь '20' лишний, если не ожидается.
        // Пока что мы это не обрабатываем как ошибку здесь, позволяя внешнему циклу решать.
    }
    return std::make_unique<ExpressionStatement>(std::move(expr));
}

// --- Методы для разбора выражений (Expressions) --- 
std::unique_ptr<IExpression> Parser::parseExpression() {
    return parseAssignment(); // Начинаем с самого низкого приоритета (присваивание)
}

std::unique_ptr<IExpression> Parser::parseAssignment() {
    std::unique_ptr<IExpression> expr = parseLogicalOr(); // Следующий уровень приоритета
    // TODO: Реализовать логику присваивания, если TokenType::EQUAL и expr - это l-value
    // if (match({TokenType::EQUAL})) { ... }
    return expr; 
}

std::unique_ptr<IExpression> Parser::parseLogicalOr() {
    std::unique_ptr<IExpression> expr = parseLogicalAnd();
    // TODO: while (match({TokenType::OR_OPERATOR})) { ... }
    return expr;
}

std::unique_ptr<IExpression> Parser::parseLogicalAnd() {
    std::unique_ptr<IExpression> expr = parseEquality();
    // TODO: while (match({TokenType::AND_OPERATOR})) { ... }
    return expr;
}

std::unique_ptr<IExpression> Parser::parseEquality() {
    std::unique_ptr<IExpression> expr = parseComparison();
    while (match({TokenType::OPERATOR_NE, TokenType::OPERATOR_EQ})) {
        Token op_token = previous();
        std::unique_ptr<IExpression> right = parseComparison();
        if (!right) return nullptr; // Ошибка разбора правой части
        expr = std::make_unique<BinaryExpression>(std::move(expr), op_token, std::move(right));
    }
    return expr;
}

std::unique_ptr<IExpression> Parser::parseComparison() {
    std::unique_ptr<IExpression> expr = parseTerm();
    while (match({TokenType::OPERATOR_GT, TokenType::OPERATOR_GE, TokenType::OPERATOR_LT, TokenType::OPERATOR_LE})) {
        Token op_token = previous();
        std::unique_ptr<IExpression> right = parseTerm();
        if (!right) return nullptr;
        expr = std::make_unique<BinaryExpression>(std::move(expr), op_token, std::move(right));
    }
    return expr;
}

std::unique_ptr<IExpression> Parser::parseTerm() {
    std::unique_ptr<IExpression> expr = parseFactor();
    while (match({TokenType::OPERATOR_MINUS, TokenType::OPERATOR_PLUS})) {
        Token op_token = previous();
        std::unique_ptr<IExpression> right = parseFactor();
        if (!right) return nullptr;
        expr = std::make_unique<BinaryExpression>(std::move(expr), op_token, std::move(right));
    }
    return expr;
}

std::unique_ptr<IExpression> Parser::parseFactor() {
    std::unique_ptr<IExpression> expr = parseUnary();
    while (match({TokenType::OPERATOR_DIV, TokenType::OPERATOR_MUL})) {
        Token op_token = previous();
        std::unique_ptr<IExpression> right = parseUnary();
        if (!right) return nullptr;
        expr = std::make_unique<BinaryExpression>(std::move(expr), op_token, std::move(right));
    }
    return expr;
}

std::unique_ptr<IExpression> Parser::parseUnary() {
    if (match({TokenType::OPERATOR_NOT, TokenType::OPERATOR_MINUS})) {
        Token op_token = previous();
        std::unique_ptr<IExpression> right = parseUnary();
        if (!right) return nullptr;
        return std::make_unique<UnaryExpression>(op_token, std::move(right));
    }
    return parsePrimary();
}

std::unique_ptr<IExpression> Parser::parsePrimary() {
    if (match({TokenType::KEYWORD_FALSE})) return std::make_unique<BooleanLiteral>(false);
    if (match({TokenType::KEYWORD_TRUE})) return std::make_unique<BooleanLiteral>(true);
    // if (match({TokenType::NIL_KEYWORD})) return std::make_unique<NilLiteral>(); // Если будет Nil

    if (match({TokenType::CONSTANT_NUM})) {
        try {
            double value = std::stod(previous().getValue());
            return std::make_unique<NumericLiteral>(value);
        } catch (const std::invalid_argument& ia) {
            // std::cerr << "Invalid number format: " << previous().getValue() << std::endl;
            return nullptr;
        } catch (const std::out_of_range& oor) {
            // std::cerr << "Number out of range: " << previous().getValue() << std::endl;
            return nullptr;
        }
    }

    if (match({TokenType::CONSTANT_STRING})) {
        return std::make_unique<StringLiteral>(previous().getValue());
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<IdentifierExpression>(previous());
    }

    if (match({TokenType::DELIMITER_LBRACKET})) {
        std::unique_ptr<IExpression> expr = parseExpression();
        if (!match({TokenType::DELIMITER_RBRACKET})) {
            // std::cerr << "Error: Expect ')' after expression." << std::endl;
            return nullptr; // Ошибка: не найдена закрывающая скобка
        }
        // TODO: Можно добавить GroupingExpression, если нужно его явно представлять в AST
        return expr; 
    }
    
    // Если ни одно из правил не сработало
    // std::cerr << "Error: Expect expression, got token: " << peek().toString() << std::endl;
    return nullptr;
}

// --- Вспомогательные методы для ошибок и синхронизации --- 
void Parser::synchronize() {
    advance(); // Пропускаем токен, вызвавший ошибку

    while (!isAtEnd()) {
        if (previous().getType() == TokenType::DELIMITER_SEMICOLON) return; // Точка с запятой часто хороший разделитель

        switch (peek().getType()) {
            // Ключевые слова, которые обычно начинают новые инструкции/объявления
            // case TokenType::CLASS_KEYWORD:
            // case TokenType::FUN_KEYWORD:
            // case TokenType::VAR_KEYWORD:
            // case TokenType::FOR_KEYWORD:
            // case TokenType::IF_KEYWORD:
            // case TokenType::WHILE_KEYWORD:
            // case TokenType::PRINT_KEYWORD: // Если есть
            // case TokenType::RETURN_KEYWORD:
            //     return;
            default:;
                // Ничего не делаем, просто пропускаем
        }
        advance();
    }
}