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

#include "Parser.h"
#include <typeinfo>
#include "Runtime.hpp"
#include "Utils.hpp"

static const std::unordered_map<std::string, Token> KEYWORDS = {
    {"if", KW_IF},       {"else", KW_ELSE},         {"while", KW_WHILE},
    {"null", KW_NULL},   {"true", KW_TRUE},         {"false", KW_FALSE},
    {"for", KW_FOR},     {"func", KW_FUNC},         {"return", KW_RETURN},
    {"break", KW_BREAK}, {"continue", KW_CONTINUE}, {"match", KW_MATCH}};

Parser::Parser(const std::string& fileName) {
    fs.open(fileName);
    if (!fs.is_open()) {
        panic("can not open source file");
    }
}

Parser::~Parser() {
    fs.close();
}

//===----------------------------------------------------------------------===//
// Parse expressions
//===----------------------------------------------------------------------===//
Expression* Parser::parsePrimaryExpr() {
    switch (getCurrentToken()) {
        case TK_IDENT: {
            auto ident = getCurrentLexeme();
            currentToken = next();
            switch (getCurrentToken()) {
                case TK_LPAREN: {
                    currentToken = next();
                    auto* val = new FunCallExpr(line, column);
                    val->funcName = ident;
                    while (getCurrentToken() != TK_RPAREN) {
                        val->args.push_back(parseExpression());
                        if (getCurrentToken() == TK_COMMA) {
                            currentToken = next();
                        }
                    }
                    assert(getCurrentToken() == TK_RPAREN);
                    currentToken = next();
                    return val;
                }
                case TK_LBRACKET: {
                    currentToken = next();
                    auto* val = new IndexExpr(line, column);
                    val->identName = ident;
                    val->index = parseExpression();
                    assert(val->index != nullptr);
                    assert(getCurrentToken() == TK_RBRACKET);
                    currentToken = next();
                    return val;
                }
                default: {
                    auto* node = new NameExpr(line, column);
                    node->identName = ident;
                    return node;
                }
            }
        }
        case TK_LBRACKET: {
            currentToken = next();
            auto* ret = new ArrayExpr(line, column);
            if (getCurrentToken() != TK_RBRACKET) {
                while (getCurrentToken() != TK_RBRACKET) {
                    ret->literal.push_back(parseExpression());
                    if (getCurrentToken() == TK_COMMA) {
                        currentToken = next();
                    }
                }
                assert(getCurrentToken() == TK_RBRACKET);
                currentToken = next();
                return ret;
            } else {
                currentToken = next();
                // It's an empty array literal
                return ret;
            }
        }
        case KW_FUNC: {
            currentToken = next();
            assert(getCurrentToken() == TK_LPAREN);
            auto* ret = new ClosureExpr(line, column);
            ret->params = parseParameterList();
            if (getCurrentToken() == TK_LBRACE) {
                ret->block = parseBlock();
            } else if (getCurrentToken() == TK_MATCH) {
                currentToken = next();
                ret->block = new Block;
                ret->block->stmts.push_back(parseStatement());
            } else {
                panic("expects => or { after closure declaration");
            }
            return ret;
        }
        case LIT_INT: {
            auto val = atoi(getCurrentLexeme().c_str());
            currentToken = next();
            auto* ret = new IntExpr(line, column);
            ret->literal = val;
            return ret;
        }
        case LIT_DOUBLE: {
            auto val = atof(getCurrentLexeme().c_str());
            currentToken = next();
            auto* ret = new DoubleExpr(line, column);
            ret->literal = val;
            return ret;
        }
        case LIT_STR: {
            auto val = getCurrentLexeme();
            currentToken = next();
            auto* ret = new StringExpr(line, column);
            ret->literal = val;
            return ret;
        }
        case LIT_CHAR: {
            auto val = getCurrentLexeme();
            currentToken = next();
            auto* ret = new CharExpr(line, column);
            ret->literal = val[0];
            return ret;
        }
        case KW_TRUE:
        case KW_FALSE: {
            auto val = (KW_TRUE == getCurrentToken());
            currentToken = next();
            auto* ret = new BoolExpr(line, column);
            ret->literal = val;
            return ret;
        }
        case KW_NULL: {
            currentToken = next();
            return new NullExpr(line, column);
        }
        case TK_LPAREN: {
            currentToken = next();
            auto val = parseExpression();
            assert(getCurrentToken() == TK_RPAREN);
            currentToken = next();
            return val;
        }
    }
    return nullptr;
}

Expression* Parser::parseUnaryExpr() {
    // !expr
    if (anyone(getCurrentToken(), TK_MINUS, TK_LOGNOT, TK_BITNOT)) {
        auto val = new BinaryExpr(line, column);
        val->opt = getCurrentToken();
        currentToken = next();
        val->lhs = parseUnaryExpr();
        return val;
    }
    // 3.14,32,"foo",'c',name,[],true,null,func(){},name.foo()
    else if (anyone(getCurrentToken(), LIT_DOUBLE, LIT_INT, LIT_STR, LIT_CHAR,
                    TK_IDENT, TK_LPAREN, TK_LBRACKET, KW_TRUE, KW_FALSE,
                    KW_NULL, KW_FUNC)) {
        Expression* prim = parsePrimaryExpr();
        if (getCurrentToken() == TK_DOT) {
            currentToken = next();
            Expression* call = parsePrimaryExpr();
            if (typeid(*call) != typeid(FunCallExpr)) {
                panic("expected a object member function call");
            }
            ((FunCallExpr*)call)->receiver = prim;
            return call;
        } else {
            return prim;
        }
    }
    return nullptr;
}

Expression* Parser::parseExpression(short oldPrecedence) {
    auto* p = parseUnaryExpr();

    // expr += expr
    if (anyone(getCurrentToken(), TK_ASSIGN, TK_PLUS_AGN, TK_MINUS_AGN,
               TK_TIMES_AGN, TK_DIV_AGN, TK_MOD_AGN)) {
        if (typeid(*p) != typeid(NameExpr) && typeid(*p) != typeid(IndexExpr)) {
            panic("can not assign to %s", typeid(*p).name());
        }
        auto* assignExpr = new AssignExpr(line, column);
        assignExpr->opt = getCurrentToken();
        assignExpr->lhs = p;
        currentToken = next();
        assignExpr->rhs = parseExpression();
        return assignExpr;
    }

    // expr % expr
    while (anyone(getCurrentToken(), TK_BITOR, TK_BITAND, TK_BITNOT, TK_LOGOR,
                  TK_LOGAND, TK_LOGNOT, TK_EQ, TK_NE, TK_GT, TK_GE, TK_LT,
                  TK_LE, TK_PLUS, TK_MINUS, TK_MOD, TK_TIMES, TK_DIV)) {
        short currentPrecedence = Parser::precedence(getCurrentToken());
        if (oldPrecedence > currentPrecedence) {
            return p;
        }
        auto tmp = new BinaryExpr(line, column);
        tmp->lhs = p;
        tmp->opt = getCurrentToken();
        currentToken = next();
        tmp->rhs = parseExpression(currentPrecedence + 1);
        p = tmp;
    }
    return p;
}

//===----------------------------------------------------------------------===//
// Parse statements and save results to runtime
//===----------------------------------------------------------------------===//
SimpleStmt* Parser::parseExpressionStmt() {
    SimpleStmt* node = nullptr;
    if (auto p = parseExpression(); p != nullptr) {
        node = new SimpleStmt(line, column);
        node->expr = p;
    }
    return node;
}

IfStmt* Parser::parseIfStmt() {
    auto* node = new IfStmt(line, column);
    currentToken = next();
    node->cond = parseExpression();
    assert(getCurrentToken() == TK_RPAREN);
    currentToken = next();
    node->block = parseBlock();
    if (getCurrentToken() == KW_ELSE) {
        currentToken = next();
        node->elseBlock = parseBlock();
    }
    return node;
}

WhileStmt* Parser::parseWhileStmt() {
    auto* node = new WhileStmt(line, column);
    currentToken = next();
    node->cond = parseExpression();
    assert(getCurrentToken() == TK_RPAREN);
    currentToken = next();
    node->block = parseBlock();
    return node;
}

Statement* Parser::parseForStmt() {
    currentToken = next();
    auto init = parseExpression();
    if (typeid(*init) == typeid(NameExpr) && getCurrentToken() == TK_COLON) {
        auto* node = new ForEachStmt(line, column);
        node->identName = dynamic_cast<NameExpr*>(init)->identName;
        currentToken = next();
        node->list = parseExpression();
        assert(getCurrentToken() == TK_RPAREN);
        currentToken = next();
        node->block = parseBlock();
        return node;
    } else {
        auto* node = new ForStmt(line, column);
        node->init = init;
        assert(getCurrentToken() == TK_SEMICOLON);
        currentToken = next();
        node->cond = parseExpression();
        assert(getCurrentToken() == TK_SEMICOLON);
        currentToken = next();
        node->post = parseExpression();
        assert(getCurrentToken() == TK_RPAREN);
        currentToken = next();
        node->block = parseBlock();
        return node;
    }
}

MatchStmt* Parser::parseMatchStmt() {
    auto* node = new MatchStmt(line, column);

    // If we met "{" after "match" keyword, we will skip consuming condition
    // expression and the match statement degenerated to normaml multi
    // conditonal checkings.
    if (getCurrentToken() == TK_LPAREN) {
        currentToken = next();
        node->cond = parseExpression();
        assert(getCurrentToken() == TK_RPAREN);
        currentToken = next();
    }

    assert(getCurrentToken() == TK_LBRACE);
    currentToken = next();

    if (getCurrentToken() != TK_RBRACE) {
        Expression* theCase = nullptr;
        Block* block = nullptr;
        do {
            theCase = parseExpression();
            assert(getCurrentToken() == TK_MATCH);
            currentToken = next();
            if (getCurrentToken() == TK_LBRACE) {
                block = parseBlock();
            } else {
                block = new Block;
                block->stmts.push_back(parseExpressionStmt());
            }

            if (typeid(*theCase) == typeid(NameExpr) &&
                dynamic_cast<NameExpr*>(theCase)->identName == "_") {
                node->matches.emplace_back(theCase, block, true);
            } else {
                node->matches.emplace_back(theCase, block, false);
            }
        } while (getCurrentToken() != TK_RBRACE);
    }
    currentToken = next();
    return node;
}

ReturnStmt* Parser::parseReturnStmt() {
    auto* node = new ReturnStmt(line, column);
    node->ret = parseExpression();
    return node;
}

Statement* Parser::parseStatement() {
    Statement* node;
    switch (getCurrentToken()) {
        case KW_IF:
            currentToken = next();
            node = parseIfStmt();
            break;
        case KW_WHILE:
            currentToken = next();
            node = parseWhileStmt();
            break;
        case KW_RETURN:
            currentToken = next();
            node = parseReturnStmt();
            break;
        case KW_BREAK:
            currentToken = next();
            node = new BreakStmt(line, column);
            break;
        case KW_CONTINUE:
            currentToken = next();
            node = new ContinueStmt(line, column);
            break;
        case KW_FOR:
            currentToken = next();
            node = parseForStmt();
            break;
        case KW_MATCH:
            currentToken = next();
            node = parseMatchStmt();
            break;
        default:
            node = parseExpressionStmt();
            break;
    }
    return node;
}

std::vector<Statement*> Parser::parseStatementList() {
    std::vector<Statement*> node;
    Statement* p;
    while ((p = parseStatement()) != nullptr) {
        node.push_back(p);
    }
    return node;
}

Block* Parser::parseBlock() {
    Block* node{new Block};
    currentToken = next();
    node->stmts = parseStatementList();
    assert(getCurrentToken() == TK_RBRACE);
    currentToken = next();
    return node;
}

std::vector<std::string> Parser::parseParameterList() {
    std::vector<std::string> node;
    currentToken = next();
    if (getCurrentToken() == TK_RPAREN) {
        currentToken = next();
        return std::move(node);
    }

    while (getCurrentToken() != TK_RPAREN) {
        if (getCurrentToken() == TK_IDENT) {
            node.push_back(getCurrentLexeme());
        } else {
            assert(getCurrentToken() == TK_COMMA);
        }
        currentToken = next();
    }
    assert(getCurrentToken() == TK_RPAREN);
    currentToken = next();
    return move(node);
}

Func* Parser::parseFuncDef(Context* context) {
    assert(getCurrentToken() == KW_FUNC);
    currentToken = next();

    // Check if function was already be defined
    if (context->hasFunction(getCurrentLexeme())) {
        panic("multiply function definitions of %s found",
              getCurrentLexeme().c_str());
    }

    auto* node = new Func;
    node->name = getCurrentLexeme();
    currentToken = next();
    assert(getCurrentToken() == TK_LPAREN);
    node->params = parseParameterList();
    node->block = parseBlock();

    return node;
}

void Parser::parse(Runtime* rt) {
    currentToken = next();
    if (getCurrentToken() == TK_EOF) {
        return;
    }
    do {
        if (getCurrentToken() == KW_FUNC) {
            auto* f = parseFuncDef(rt);
            rt->addFunction(f->name, f);
        } else {
            rt->addStatement(parseStatement());
        }
    } while (getCurrentToken() != TK_EOF);
}

//===----------------------------------------------------------------------===//
// Implementation of lexer within simple next() function
//===----------------------------------------------------------------------===//
std::tuple<Token, std::string> Parser::next() {
    char c = getNextChar();

    if (c == EOF) {
        return std::make_tuple(TK_EOF, "");
    }
    if (anyone(c, ' ', '\n', '\r', '\t')) {
        while (anyone(c, ' ', '\n', '\r', '\t')) {
            if (c == '\n') {
                line++;
                column = 0;
            }
            c = getNextChar();
        }
        if (c == EOF) {
            return std::make_tuple(TK_EOF, "");
        }
    }

    if (c == '#') {
    another_comment:
        while (c != '\n' && c != EOF) {
            c = getNextChar();
        }
        // consume newlines
        while (c == '\n') {
            line++;
            column = 0;
            c = getNextChar();
        }
        if (c == '#') {
            goto another_comment;
        }
        if (c == EOF) {
            return std::make_tuple(TK_EOF, "");
        }
    }

    if (c >= '0' && c <= '9') {
        std::string lexeme{c};
        bool isDouble = false;
        char cn = peekNextChar();
        while ((cn >= '0' && cn <= '9') || (!isDouble && cn == '.')) {
            if (c == '.') {
                isDouble = true;
            }
            c = getNextChar();
            cn = peekNextChar();
            lexeme += c;
        }
        return !isDouble ? make_tuple(LIT_INT, lexeme)
                         : make_tuple(LIT_DOUBLE, lexeme);
    }
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
        std::string lexeme{c};
        char cn = peekNextChar();
        while ((cn >= 'a' && cn <= 'z') || (cn >= 'A' && cn <= 'Z') ||
               (cn >= '0' && cn <= '9') || cn == '_') {
            c = getNextChar();
            lexeme += c;
            cn = peekNextChar();
        }
        auto result = KEYWORDS.find(lexeme);
        return result != KEYWORDS.end()
                   ? std::make_tuple(result->second, lexeme)
                   : std::make_tuple(TK_IDENT, lexeme);
    }

    switch (c) {
        case '\'': {
            std::string lexeme;
            char nextChar = getNextChar();
            if (nextChar != '\'') {
                lexeme += nextChar;
                if (peekNextChar() != '\'') {
                    panic(
                        "a character literal should surround with "
                        "single-quote");
                }
                c = getNextChar();
            }
            return std::make_tuple(LIT_CHAR, lexeme);
        }
        case '"': {
            std::string lexeme;
            char cn = peekNextChar();
            while (cn != '"') {
                c = getNextChar();
                lexeme += c;
                cn = peekNextChar();
            }
            c = getNextChar();
            return std::make_tuple(LIT_STR, lexeme);
        }
        case '[': {
            return std::make_tuple(TK_LBRACKET, "[");
        }
        case ']': {
            return std::make_tuple(TK_RBRACKET, "]");
        }
        case '{': {
            return std::make_tuple(TK_LBRACE, "{");
        }
        case '}': {
            return std::make_tuple(TK_RBRACE, "}");
        }
        case '(': {
            return std::make_tuple(TK_LPAREN, "(");
        }
        case ')': {
            return std::make_tuple(TK_RPAREN, ")");
        }
        case ',': {
            return std::make_tuple(TK_COMMA, ",");
        }
        case ';': {
            return std::make_tuple(TK_SEMICOLON, ";");
        }
        case ':': {
            return std::make_tuple(TK_COLON, ":");
        }
        case '+': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_PLUS_AGN, "+=");
            }
            return std::make_tuple(TK_PLUS, "+");
        }
        case '-': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_MINUS_AGN, "-=");
            }
            return std::make_tuple(TK_MINUS, "-");
        }
        case '*': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_TIMES_AGN, "*=");
            }
            return std::make_tuple(TK_TIMES, "*");
        }
        case '/': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_DIV_AGN, "/=");
            }
            return std::make_tuple(TK_DIV, "/");
        }
        case '%': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_MOD_AGN, "%=");
            }
            return std::make_tuple(TK_MOD, "%");
        }
        case '~': {
            return std::make_tuple(TK_BITNOT, "~");
        }
        case '.': {
            return std::make_tuple(TK_DOT, ".");
        }
        case '=': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_EQ, "==");
            } else if (peekNextChar() == '>') {
                c = getNextChar();
                return std::make_tuple(TK_MATCH, "=>");
            }
            return std::make_tuple(TK_ASSIGN, "=");
        }
        case '!': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_NE, "!=");
            }
            return std::make_tuple(TK_LOGNOT, "!");
        }
        case '|': {
            if (peekNextChar() == '|') {
                c = getNextChar();
                return std::make_tuple(TK_LOGOR, "||");
            }
            return std::make_tuple(TK_BITOR, "|");
        }
        case '&': {
            if (peekNextChar() == '&') {
                c = getNextChar();
                return std::make_tuple(TK_LOGAND, "&&");
            }
            return std::make_tuple(TK_BITAND, "&");
        }
        case '>': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_GE, ">=");
            }
            return std::make_tuple(TK_GT, ">");
        }
        case '<': {
            if (peekNextChar() == '=') {
                c = getNextChar();
                return std::make_tuple(TK_LE, "<=");
            }
            return std::make_tuple(TK_LT, "<");
        }
        default: {
            panic("unknown token %c", c);
        }
    }
}

short Parser::precedence(Token op) {
    switch (op) {
        case TK_LOGOR:
            return 1;
        case TK_LOGAND:
            return 2;
        case TK_EQ:
        case TK_NE:
        case TK_GT:
        case TK_GE:
        case TK_LT:
        case TK_LE:
            return 3;
        case TK_PLUS:
        case TK_MINUS:
        case TK_BITOR:
            return 4;
        case TK_TIMES:
        case TK_MOD:
        case TK_DIV:
        case TK_BITAND:
            return 5;
        default:
            // Lowest precedence
            return 0;
    }
}
