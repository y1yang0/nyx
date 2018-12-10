#include "Debug.h"
#include "Nyx.h"
#include "Parser.h"

int main() {
    Parser parser("source.nyx");
    parser.parse();
    system("pause");
    return 0;
}
