#include "Interpreter.h"

int main() {
    // Parser::printLex("source.nyx");
    NyxInterpreter nyx("hello.nyx");
    nyx.execute();
    system("pause");
    return 0;
}
