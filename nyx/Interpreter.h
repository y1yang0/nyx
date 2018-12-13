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

private:
    void parseCommandOption(int argc, char* argv) {}

private:
    Parser* p;
};
