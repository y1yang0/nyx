#include <string.h>
#include <iostream>
#include "Interpreter.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Feed your *.nyx source file to interpreter!\n";
        return EXIT_FAILURE;
    }

    NyxInterpreter nyx(argv[1]);
    nyx.execute();
    return 0;
}
