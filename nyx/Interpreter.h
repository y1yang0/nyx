#pragma once
#include <memory>
#include "Nyx.hpp"
#include "Parser.h"

class NyxInterpreter {
public:
    explicit NyxInterpreter(const std::string& fileName);
    ~NyxInterpreter();

public:
    void execute();

    static void enterContext(std::deque<nyx::LocalContext*>& ctxChain);

    static void leaveContext(std::deque<nyx::LocalContext*>& ctxChain);

private:
    void parseCommandOption(int argc, char* argv) {}

private:
    Parser* p;
};
