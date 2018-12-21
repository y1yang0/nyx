#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include "Ast.h"
#include "Nyx.hpp"

namespace nyx {
class Parser {
public:
    explicit Parser(const std::string& fileName);
    ~Parser();

public:
    void parse(nyx::Runtime* rt);
    static void printLex(const std::string& fileName);

private:
    Expression* parsePrimaryExpr();
    Expression* parseUnaryExpr();
    // default to lowest precedence(0) + 1
    Expression* parseExpression(short oldPrecedence = 1);
    ExpressionStmt* parseExpressionStmt();
    IfStmt* parseIfStmt();
    WhileStmt* parseWhileStmt();
    Statement* parseForStmt();
    ReturnStmt* parseReturnStmt();
    Statement* parseStatement();
    std::vector<Statement*> parseStatementList();
    Block* parseBlock();
    std::vector<std::string> parseParameterList();
    nyx::Function* parseFuncDef(nyx::Context* context);

private:
    short precedence(Token op);

    std::tuple<Token, std::string> next();

    inline char getNextChar() {
        column++;
        return static_cast<char>(fs.get());
    }

    inline char peekNextChar() { return static_cast<char>(fs.peek()); }

    inline Token getCurrentToken() const {
        return std::get<Token>(currentToken);
    }
    inline std::string getCurrentLexeme() const {
        return std::get<std::string>(currentToken);
    }

private:
    const std::unordered_map<std::string, Token> keywords;

    std::tuple<Token, std::string> currentToken;

    std::fstream fs;

    int line = 1;

    int column = 0;
};
}  // namespace nyx
