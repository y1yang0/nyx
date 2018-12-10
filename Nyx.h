#pragma once

#include <any>
#include <string>
#include <vector>
using namespace std;
struct Block;
struct Statement;

namespace nyx {
enum ValueType { NyxInt, NyxDouble, NyxString, NyxBool, NyxNull };
}

struct Function {
    explicit Function() {}

    string name;
    vector<string> params;
    shared_ptr<Block> block;
};

struct Value {
    explicit Value() {}
    explicit Value(nyx::ValueType type) : type(type) {}
    explicit Value(nyx::ValueType type, any data) : type(type), data(data) {}

    nyx::ValueType type;
    any data;
};

struct Variable {
    explicit Variable() {}

    string name;
    Value value;
};

struct GlobalContext {
    explicit GlobalContext() {}

    vector<shared_ptr<Variable>> vars;
    vector<shared_ptr<Function>> funcs;
    vector<shared_ptr<Statement>> stmts;
};

struct LocalContext {
    vector<shared_ptr<Variable>> vars;
};
