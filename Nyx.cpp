#include "Interpreter.h"
#include "Nyx.h"
#include "Parser.h"

int main() {
    // Parser::printLex("source.nyx");
    NyxInterpreter nyx("source.nyx");
    nyx.execute();
    system("pause");
    return 0;
}
