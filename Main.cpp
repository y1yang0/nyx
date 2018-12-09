#include "Debug.h"
#include "NyxInterpreter.h"
#include "Parser.h"

int main() {
    shared_ptr<NyxInterpreter> nyx(new NyxInterpreter);
    Parser parser("source.nyx");
    parser.parse(nyx);
    system("pause");
    return 0;
}
