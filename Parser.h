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
    explicit Parser(const string& fileName);
    GlobalContext* parse();
    static void printLex(const string& fileName);

private:
    Expression* parsePrimaryExpr();
    Expression* parseUnaryExpr();
    BinaryExpr* parseExpression();
    ExpressionStmt* parseExpressionStmt();
    IfStmt* parseIfStmt();
    WhileStmt* parseWhileStmt();
    Statement* parseStatement();
    vector<Statement*> parseStatementList();
    Block* parseBlock();
    vector<string> parseParameterList();
    Function* parseFuncDef();

private:
    tuple<Token, string> next();
    tuple<Token, string> expect(Token tk);

    inline Token getCurrentToken() const { return get<0>(currentToken); }
    inline string getCurrentLexeme() const { return get<1>(currentToken); }

private:
    const map<string, Token> keywords{{"if", KW_IF},       {"while", KW_WHILE},
                                      {"null", KW_NULL},   {"true", KW_TRUE},
                                      {"false", KW_FALSE}, {"for", KW_FALSE},
                                      {"func", KW_FUNC}};

    tuple<Token, string> currentToken;

    GlobalContext* context;

    fstream fs;
};
