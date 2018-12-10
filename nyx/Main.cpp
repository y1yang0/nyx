#include "Interpreter.h"
#include <iostream>

int main(int argc, char*argv[]) {
    if(argc<2){
        cerr<<"Feed your *.nyx source file to interpreter!\n";
        return EXIT_FAILURE;
    }

    NyxInterpreter nyx(argv[1]);
    nyx.execute();
    return 0;
}
