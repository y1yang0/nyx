#include <string.h>
#include <iostream>
#include "Interpreter.h"
#include "Utils.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        panic("Feed your *.nyx source file to interpreter!\n");
    }

    auto* rt = new nyx::Runtime;

    nyx::Parser parser(argv[1]);
    parser.parse(rt);
    nyx::Interpreter nyx;
    nyx.execute(rt);

    return 0;
}
