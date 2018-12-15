#include <string.h>
#include <iostream>
#include "Interpreter.h"
#include "Utils.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        panic("Feed your *.nyx source file to interpreter!\n");
    }

    Interpreter nyx(argv[1]);
    nyx.execute();
    return 0;
}
