#include <iostream>
#include <vector>
#include "Ast.h"
#include "Builtin.h"
#include "Nyx.h"

nyx::Value print(nyx::GlobalContext* ctx, std::vector<Value> args) {
    for (auto arg : args) {
        switch (arg.type) {
            case nyx::NyxBool:
                std::cout << std::boolalpha << std::any_cast<bool>(arg.data) << "\n";
                break;
            case nyx::NyxDouble:
                std::cout << std::any_cast<double>(arg.data) << "\n";
                break;
            case nyx::NyxInt:
                std::cout << std::any_cast<int>(arg.data) << "\n";
                break;
            case nyx::NyxString:
                std::cout << std::any_cast<string>(arg.data) << "\n";
                break;
            case nyx::NyxNull:
                std::cout << "null\n";
                break;
            default:
                std::cout << "<untyped " << &arg.data << ">\n";
                break;
        }
    }
    return nyx::Value(nyx::NyxInt, (int)args.size());
}
