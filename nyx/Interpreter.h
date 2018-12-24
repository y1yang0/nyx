#pragma once
#include <memory>
#include "Nyx.hpp"
#include "Parser.h"

namespace nyx {
class Interpreter {
public:
    void execute(Runtime* rt);

public:
    static void enterContext(std::deque<Context*>& ctxChain);

    static void leaveContext(std::deque<Context*>& ctxChain);

    static Value callFunction(Runtime* rt, Function* f,
                              std::deque<Context*> previousCtxChain,
                              std::vector<Expression*> args);

    static Value calcBinaryExpr(Value lhs, Token opt, Value rhs, int line,
                                int column);

    static Value calcUnaryExpr(Value& lhs, Token opt, int line, int column);
    static Value assignSwitch(Token opt, Value lhs, Value rhs);

private:
    void parseCommandOption(int argc, char* argv) {}

private:
    std::deque<Context*> ctxChain;
    Runtime* rt;
};

}  // namespace nyx
