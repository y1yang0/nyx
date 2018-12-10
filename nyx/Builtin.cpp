#include <iostream>
#include <vector>
#include "Ast.h"
#include "Builtin.h"
#include "Nyx.h"
using namespace std;

Value print(GlobalContext* ctx, std::vector<Value> args) {
    for (auto arg : args) {
        switch (arg.type) {
            case nyx::NyxBool:
                cout << boolalpha << any_cast<bool>(arg.data);
                break;
            case nyx::NyxDouble:
                cout << any_cast<double>(arg.data);
                break;
            case nyx::NyxInt:
                cout << any_cast<int>(arg.data);
                break;
            case nyx::NyxString:
                cout << any_cast<string>(arg.data);
                break;
            case nyx::NyxNull:
                cout << "null";
                break;
            default:
                cout << "<addr " << &arg.data << ">";
                break;
        }
    }
    return Value(nyx::NyxNull);
}
