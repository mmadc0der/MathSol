#pragma once

#include "../../lexer/include/token.hpp"
#include "expression.hpp" // Содержит IExpression, Value и конкретные выражения
#include "statement.hpp"  // Содержит IStatement и конкретные инструкции
#include <vector>
#include <memory> // Для std::unique_ptr

class Parser {
public:
    // Конструктор принимает все токены для парсинга
    explicit Parser(const std::vector<Token>& tokens);

    // Главный метод парсинга, возвращает список инструкций (AST)
    std::vector<std::unique_ptr<IStatement>> parse();

private:
    const std::vector<Token>& tokens_; // Ссылка на исходные токены
    size_t currentTokenIndex_;        // Индекс текущего токена для разбора
    // Environment* environment_; // Если понадобится доступ к окружению во время парсинга

    // Вспомогательные методы парсера
    Token advance();                 // Передвигает указатель на следующий токен и возвращает предыдущий
    Token peek() const;              // Возвращает текущий токен без сдвига указателя
    Token previous() const;          // Возвращает предыдущий токен
    bool isAtEnd() const;            // Проверяет, достигнут ли конец потока токенов
    bool check(TokenType type) const; // Проверяет тип текущего токена
    bool match(const std::vector<TokenType>& types); // Проверяет, соответствует ли текущий токен одному из типов, и если да, то сдвигает указатель

    // Методы для разбора конкретных грамматических конструкций
    // Все они возвращают std::unique_ptr на соответствующий узел AST или nullptr при ошибке

    // Инструкции (Statements)
    std::unique_ptr<IStatement> parseDeclaration(); // Для переменных, функций (если будут)
    std::unique_ptr<IStatement> parseStatement();
    std::unique_ptr<IStatement> parseExpressionStatement();
    // std::unique_ptr<IStatement> parseIfStatement();        // TODO
    // std::unique_ptr<IStatement> parseWhileStatement();     // TODO
    // std::unique_ptr<IStatement> parseForStatement();       // TODO
    // std::unique_ptr<IStatement> parseFunctionDeclaration(const std::string& kind); // TODO
    // std::unique_ptr<IStatement> parseReturnStatement();    // TODO

    // Выражения (Expressions)
    std::unique_ptr<IExpression> parseExpression();
    std::unique_ptr<IExpression> parseAssignment();      // x = 10
    std::unique_ptr<IExpression> parseLogicalOr();       // or
    std::unique_ptr<IExpression> parseLogicalAnd();      // and
    std::unique_ptr<IExpression> parseEquality();        // == !=
    std::unique_ptr<IExpression> parseComparison();      // < > <= >=
    std::unique_ptr<IExpression> parseTerm();            // + -
    std::unique_ptr<IExpression> parseFactor();          // * /
    std::unique_ptr<IExpression> parseUnary();           // ! -
    // std::unique_ptr<IExpression> parseCall();             // func(args)
    std::unique_ptr<IExpression> parsePrimary();         // Литералы, группировка, идентификаторы

    // Вспомогательные методы для ошибок и синхронизации
    void synchronize(); // Для восстановления после ошибки парсинга
    // ParseError error(const Token& token, const std::string& message); // TODO: Определить ParseError

    // Старые методы, которые нужно будет адаптировать или заменить:
    // bool parseBooleanLiteral(); -> станет частью parsePrimary
    // bool parseIntegerLiteral(); -> станет частью parsePrimary
    // bool parseIdentifier();     -> станет частью parsePrimary
    // bool parseUnaryExpression(); -> заменен на parseUnary
    // bool parseBinaryExpression(int precedence); -> заменен цепочкой parseEquality, parseComparison и т.д.
};