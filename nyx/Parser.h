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

class Parser {
public:
    explicit Parser(const std::string& fileName);
    ~Parser();

public:
    void parse(nyx::Runtime* rt, nyx::Context* context);
    static void printLex(const std::string& fileName);
    short precedence(Token op);

private:
    Expression* parsePrimaryExpr();
    Expression* parseUnaryExpr();
    Expression* parseExpression(
        short oldPrecedence = 1 /*default to lowest precedence(0) + 1*/);
    ExpressionStmt* parseExpressionStmt();
    IfStmt* parseIfStmt();
    WhileStmt* parseWhileStmt();
    Statement* parseStatement();
    std::vector<Statement*> parseStatementList();
    Block* parseBlock();
    std::vector<std::string> parseParameterList();
    nyx::Function* parseFuncDef(nyx::Context* context);

private:
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
    const std::map<std::string, Token> keywords;

    std::tuple<Token, std::string> currentToken;

    std::fstream fs;

    int line = 1;

    int column = 0;
};
