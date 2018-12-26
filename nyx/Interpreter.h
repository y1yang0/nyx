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

    static Value calcBinaryExpr(const Value& lhs, Token opt, const Value& rhs,
                                int line, int column);

    static Value calcUnaryExpr(const Value& lhs, Token opt, int line,
                               int column);
    static Value assignSwitch(Token opt, const Value& lhs, const Value& rhs);

private:
    void parseCommandOption(int argc, char* argv) {}

private:
    std::deque<Context*> ctxChain;
    Runtime* rt;
};

}  // namespace nyx
