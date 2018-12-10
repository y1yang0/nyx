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
    shared_ptr<GlobalContext> parse();
    static void printLex(const string& fileName);

private:
    shared_ptr<Expression> parsePrimaryExpr();
    shared_ptr<Expression> parseUnaryExpr();
    shared_ptr<BinaryExpr> parseExpression();
    shared_ptr<ExpressionStmt> parseExpressionStmt();
    shared_ptr<IfStmt> parseIfStmt();
    shared_ptr<WhileStmt> parseWhileStmt();
    shared_ptr<Statement> parseStatement();
    vector<shared_ptr<Statement>> parseStatementList();
    shared_ptr<Block> parseBlock();
    vector<string> parseParameterList();
    shared_ptr<Function> parseFuncDef();

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

    shared_ptr<GlobalContext> context;

    fstream fs;
};
