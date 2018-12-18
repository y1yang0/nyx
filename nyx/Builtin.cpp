#include <iostream>
#include <vector>
#include "Ast.h"
#include "Builtin.h"
#include "Nyx.hpp"
#include "Utils.hpp"

nyx::Value nyx_builtin_print(nyx::Runtime* rt,
                             std::deque<nyx::Context*> ctxChain,
                             std::vector<nyx::Value> args) {
    for (auto arg : args) {
        std::cout << valueToStdString(arg);
    }
    return nyx::Value(nyx::Int, (int)args.size());
}

nyx::Value nyx_builtin_println(nyx::Runtime* rt,
                               std::deque<nyx::Context*> ctxChain,
                               std::vector<nyx::Value> args) {
    if (args.size() != 0) {
        for (auto arg : args) {
            std::cout << valueToStdString(arg) << "\n";
        }
    } else {
        std::cout << "\n";
    }

    return nyx::Value(nyx::Int, (int)args.size());
}

nyx::Value nyx_builtin_input(nyx::Runtime* rt,
                             std::deque<nyx::Context*> ctxChain,
                             std::vector<nyx::Value> args) {
    nyx::Value result{nyx::String};

    std::string str;
    std::cin >> str;
    result.data = std::make_any<std::string>(std::move(str));
    return result;
}

nyx::Value nyx_builtin_typeof(nyx::Runtime* rt,
                              std::deque<nyx::Context*> ctxChain,
                              std::vector<nyx::Value> args) {
    if (args.size() != 1) {
        panic("ArgumentError: expects one argument but got %d", args.size());
    }
    nyx::Value result(nyx::String);
    switch (args[0].type) {
        case nyx::Bool:
            result.set<std::string>("bool");
            break;
        case nyx::Double:
            result.set<std::string>("double");
            break;
        case nyx::Int:
            result.set<std::string>("int");
            break;
        case nyx::String:
            result.set<std::string>("string");
            break;
        case nyx::Null:
            result.set<std::string>("null");
            break;
        case nyx::Char:
            result.set<std::string>("char");
            break;
        default:
            panic("TypeError: unknown type!");
    }
    return result;
}

nyx::Value nyx_builtin_length(nyx::Runtime* rt,
                              std::deque<nyx::Context*> ctxChain,
                              std::vector<nyx::Value> args) {
    if (args.size() != 1) {
        panic("ArgumentError: expects one argument but got %d", args.size());
    }
    if (args[0].type != nyx::String) {
        panic("TypeError: unexpected type of arguments, requires string type");
    }
    return nyx::Value(nyx::Int,
                      std::make_any<int>(args[0].cast<std::string>().length()));
}
