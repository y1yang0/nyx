// MIT License
//
// Copyright (c) 2018-2023 y1yang0 <kelthuzadx@qq.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include "Ast.h"
#include "Runtime.hpp"

//===----------------------------------------------------------------------===//
// Parse source file to AST nodes for further use
//===----------------------------------------------------------------------===//
class Parser {
public:
    explicit Parser(const std::string& fileName);

    ~Parser();

public:
    void parse(Runtime* rt);
    std::tuple<Token, std::string> next();

private:
    Expression* parsePrimaryExpr();

    Expression* parseUnaryExpr();

    // default to lowest precedence(0) + 1
    Expression* parseExpression(short oldPrecedence = 1);

    SimpleStmt* parseExpressionStmt();

    IfStmt* parseIfStmt();

    WhileStmt* parseWhileStmt();

    Statement* parseForStmt();

    MatchStmt* parseMatchStmt();

    ReturnStmt* parseReturnStmt();

    Statement* parseStatement();

    std::vector<Statement*> parseStatementList();

    Block* parseBlock();

    std::vector<std::string> parseParameterList();

    Func* parseFuncDef(Context* context);

private:
    short precedence(Token op);

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
    std::tuple<Token, std::string> currentToken;

    std::fstream fs;

    int line = 1;

    int column = 0;
};
