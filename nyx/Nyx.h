#pragma once

#include <stdio.h>
#include <any>
#include <string>
#include <unordered_map>
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
    Block* block;
};

struct Value {
    explicit Value() {}
    explicit Value(nyx::ValueType type) : type(type) {}
    explicit Value(nyx::ValueType type, any data) : type(type), data(data) {}

    inline bool isNyxNull()const { return type == nyx::NyxNull; }

    nyx::ValueType type;
    any data;
};

struct Variable {
    explicit Variable() {}

    string name;
    Value value;
};

struct LocalContext {
    explicit LocalContext() {}

    vector<Variable*> vars;
};
struct GlobalContext : public LocalContext {
    explicit GlobalContext();

    vector<Function*> funcs;
    vector<Statement*> stmts;
    unordered_map<string, Value (*)(GlobalContext*, vector<Value>)> builtin;
};
