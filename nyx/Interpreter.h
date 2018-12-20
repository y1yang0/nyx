#pragma once
#include <memory>
#include "Nyx.hpp"
#include "Parser.h"

class Interpreter {
public:
    explicit Interpreter(const std::string& fileName);
    ~Interpreter();

public:
    void execute();

public:
    static void enterContext(std::deque<nyx::Context*>& ctxChain);

    static void leaveContext(std::deque<nyx::Context*>& ctxChain);

    static nyx::Value callFunction(nyx::Runtime* rt, nyx::Function* f,
                                   std::deque<nyx::Context*> previousCtxChain,
                                   std::vector<Expression*> args);

    static nyx::Value calcBinaryExpr(nyx::Value lhs, Token opt, Value rhs,
                                     int line, int column);

    static nyx::Value calcUnaryExpr(nyx::Value& lhs, Token opt, int line,
                                    int column);
    static nyx::Value assignSwitch(Token opt, nyx::Value lhs, nyx::Value rhs);

private:
    void parseCommandOption(int argc, char* argv) {}

private:
    std::deque<nyx::Context*> ctxChain;
    nyx::Runtime* rt;
    Parser* p;
};
