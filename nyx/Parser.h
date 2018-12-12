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
    explicit Parser(const std::string& fileName, nyx::GlobalContext* context);
    ~Parser();

public:
    nyx::GlobalContext* parse();
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
    nyx::Function* parseFuncDef();

private:
    std::tuple<Token, std::string> next();

    inline char getNextChar() {
        columnCount++;
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

    nyx::GlobalContext* context;

    std::fstream fs;

    int lineCount = 1;

    int columnCount = 0;
};
