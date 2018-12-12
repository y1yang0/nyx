#include <iostream>
#include <vector>
#include "Ast.h"
#include "Builtin.h"
#include "Nyx.h"

nyx::Value nyx_builtin_print(nyx::GlobalContext* ctx, std::vector<Value> args) {
    for (auto arg : args) {
        switch (arg.type) {
            case nyx::NyxBool:
                std::cout << std::boolalpha << std::any_cast<bool>(arg.data)
                          << "\n";
                break;
            case nyx::NyxDouble:
                std::cout << std::any_cast<double>(arg.data) << "\n";
                break;
            case nyx::NyxInt:
                std::cout << std::any_cast<int>(arg.data) << "\n";
                break;
            case nyx::NyxString:
                std::cout << std::any_cast<std::string>(arg.data) << "\n";
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

nyx::Value nyx_builtin_typeof(nyx::GlobalContext* ctx,
                              std::vector<nyx::Value> args) {
    if (args.size() != 1) {
        throw std::runtime_error("typeof expects one argument but got" +
                                 args.size());
    }
    switch (args[0].type) {
        case nyx::NyxBool:
            return nyx::Value(nyx::NyxString,
                              std::make_any<std::string>("bool"));
        case nyx::NyxDouble:
            return nyx::Value(nyx::NyxString,
                              std::make_any<std::string>("double"));
        case nyx::NyxInt:
            return nyx::Value(nyx::NyxString,
                              std::make_any<std::string>("int"));
        case nyx::NyxString:
            return nyx::Value(nyx::NyxString,
                              std::make_any<std::string>("string"));
        case nyx::NyxNull:
            return nyx::Value(nyx::NyxString,
                              std::make_any<std::string>("null"));
        default:
            return nyx::Value(nyx::NyxString,
                              std::make_any<std::string>("unknown"));
    }
}
