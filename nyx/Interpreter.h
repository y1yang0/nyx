#pragma once
#include <memory>
#include "Nyx.hpp"
#include "Parser.h"

class Interpreter {
public:
    explicit Interpreter(const std::string& fileName);
    ~Interpreter();

public:
    void prepareContext();

    void execute();

    static void enterContext(std::deque<nyx::Context*>& ctxChain);

    static void leaveContext(std::deque<nyx::Context*>& ctxChain);

private:
    void parseCommandOption(int argc, char* argv) {}

private:
    std::deque<nyx::Context*> ctxChain;
    nyx::Runtime* rt;
    Parser* p;
};
