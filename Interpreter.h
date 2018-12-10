#pragma once
#include <memory>
#include "Nyx.h"
#include "Parser.h"

using namespace std;

class NyxInterpreter {
public:
    explicit NyxInterpreter(const string& fileName) : p(fileName) {}

    void execute();

private:
    Parser p;
};
