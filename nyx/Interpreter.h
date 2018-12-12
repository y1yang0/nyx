#pragma once
#include <memory>
#include "Nyx.h"
#include "Parser.h"

class NyxInterpreter {
public:
    explicit NyxInterpreter(const std::string& fileName) : p(fileName) {}

    void execute();

private:
    Parser p;
};
