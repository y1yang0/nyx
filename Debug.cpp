#include <string>
#include <tuple>
#include "Debug.h"
#include "Parser.h"

void printLex(Parser& p) {
    std::tuple<NyxToken, std::string> tk;
    do {
        tk = p.next();
        std::cout << "[" << std::get<0>(tk) << "," << std::get<1>(tk) << "]\n";
    } while (get<0>(tk) != TK_EOF);
}
