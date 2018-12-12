#pragma once
#include <memory>
#include "Nyx.h"
#include "Parser.h"

class NyxInterpreter {
public:
    explicit NyxInterpreter(const std::string& fileName);
    ~NyxInterpreter();

public:
    void execute();

private:
    void parseCommandOption(int argc, char* argv) {}

private:
    nyx::GlobalContext* context;

    Parser* p;

    bool printHelp = false;

    bool printLex = false;

    bool printAst = false;
};
