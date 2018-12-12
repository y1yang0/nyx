#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include "Ast.h"
#include "Nyx.h"

class Parser {
public:
    explicit Parser(const std::string& fileName);
    nyx::GlobalContext* parse();
    static void printLex(const std::string& fileName);
    inline short precedence(Token op);

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
    nyx::Function* parseFuncDef();

private:
    std::tuple<Token, std::string> next();

    inline Token getCurrentToken() const {
        return std::get<Token>(currentToken);
    }
    inline std::string getCurrentLexeme() const {
        return std::get<std::string>(currentToken);
    }

private:
    const std::map<std::string, Token> keywords{
        {"if", KW_IF},     {"while", KW_WHILE}, {"null", KW_NULL},
        {"true", KW_TRUE}, {"false", KW_FALSE}, {"for", KW_FALSE},
        {"func", KW_FUNC}};

    std::tuple<Token, std::string> currentToken;

    nyx::GlobalContext* context;

    std::fstream fs;
};
