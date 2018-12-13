#include <iostream>
#include <vector>
#include "Ast.h"
#include "Builtin.h"
#include "Nyx.hpp"
#include "Utils.hpp"

nyx::Value nyx_builtin_print(nyx::GlobalContext* ctx, std::vector<Value> args) {
    for (auto arg : args) {
        std::cout << valueToStdString(arg) << "\n";
    }
    return nyx::Value(nyx::Int, (int)args.size());
}

nyx::Value nyx_builtin_input(nyx::GlobalContext* ctx, std::vector<Value> args) {
    nyx::Value result{nyx::String};

    std::string str;
    std::cin >> str;
    result.data = std::make_any<std::string>(std::move(str));
    return result;
}

nyx::Value nyx_builtin_typeof(nyx::GlobalContext* ctx,
                              std::vector<nyx::Value> args) {
    if (args.size() != 1) {
        throw std::runtime_error("typeof expects one argument but got" +
                                 args.size());
    }
    switch (args[0].type) {
        case nyx::Bool:
            return nyx::Value(nyx::String, std::make_any<std::string>("bool"));
        case nyx::Double:
            return nyx::Value(nyx::String,
                              std::make_any<std::string>("double"));
        case nyx::Int:
            return nyx::Value(nyx::String, std::make_any<std::string>("int"));
        case nyx::String:
            return nyx::Value(nyx::String,
                              std::make_any<std::string>("string"));
        case nyx::Null:
            return nyx::Value(nyx::String, std::make_any<std::string>("null"));
        default:
            return nyx::Value(nyx::String,
                              std::make_any<std::string>("unknown"));
    }
}
