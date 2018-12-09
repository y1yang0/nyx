#pragma once

#include <cassert>
#include <fstream>
#include <iostream>s
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include "NyxInterpreter.h"

class Parser {
    friend void printLex(Parser& fs);

public:
    explicit Parser(const string fileName) { fs.open(fileName); }

    vector<shared_ptr<Statement>> parseStatementList() {
        vector<shared_ptr<Statement>> node;
        return node;
    }

    shared_ptr<Block> parseBlock() {
        shared_ptr<Block> node;
        expect(TK_LBRACE);
        node->stmts = parseStatementList();
        return node;
    }

    vector<string> parseParameterList() {
        vector<string> node;
        next();
        if (getCurrentToken() == TK_RPAREN) {
            return move(node);
        }

        while (getCurrentToken() != TK_RPAREN) {
            if (getCurrentToken() == TK_IDENT) {
                node.push_back(getCurrentLexeme());
            } else {
                assert(getCurrentToken() == TK_COMMA);
            }
            next();
        }
        return move(node);
    }

    shared_ptr<Function> parseFuncDef() {
        assert(getCurrentToken() == KW_FUNC);

        shared_ptr<Function> node;
        auto [t, id] = expect(TK_IDENT);
        node->name = id;
        expect(TK_LPAREN);
        node->params = parseParameterList();
        node->block = parseBlock();

        return node;
    }

    shared_ptr<Statement> parseStatement() {}

    void parse(shared_ptr<NyxInterpreter>& context) {
        // Set up parsing context
        this->context = context;

        // Do parsing
        do {
            currentToken = next();
            if (getCurrentToken() == KW_FUNC) {
                context->funcs.push_back(parseFuncDef());
            } else {
                context->stmts.push_back(parseStatement());
            }
        } while (getCurrentToken() != TK_EOF);
    }

private:
    tuple<NyxToken, string> next();
    tuple<NyxToken, string> expect(NyxToken tk);

    inline NyxToken getCurrentToken() const { return get<0>(currentToken); }
    inline string getCurrentLexeme() const { return get<1>(currentToken); }

private:
    const map<string, NyxToken> keywords{
        {"if", KW_IF},     {"while", KW_WHILE}, {"null", KW_NULL},
        {"true", KW_TRUE}, {"false", KW_FALSE}, {"for", KW_FALSE},
        {"func", KW_FUNC}};

    tuple<NyxToken, string> currentToken;

    shared_ptr<NyxInterpreter> context;

    fstream fs;
};
