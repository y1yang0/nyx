#pragma once

#include <stdio.h>
#include <any>
#include <string>
#include <unordered_map>
#include <vector>

struct Block;
struct Statement;

namespace nyx {
enum ValueType { NyxInt, NyxDouble, NyxString, NyxBool, NyxNull };

struct Function {
    explicit Function() = default;

    std::string name;
    std::vector<std::string> params;
    Block* block{};
};

struct Value {
    explicit Value() = default;
    explicit Value(nyx::ValueType type) : type(type) {}
    explicit Value(nyx::ValueType type, std::any data)
        : type(type), data(std::move(data)) {}

    inline bool isNyxNull() const { return type == nyx::NyxNull; }

    Value operator+(Value rhs);
    Value operator-(Value rhs);
    Value operator*(Value rhs);
    Value operator/(Value rhs);
    Value operator%(Value rhs);

    Value operator&&(Value rhs);
    Value operator||(Value rhs);

    Value operator==(Value rhs);
    Value operator!=(Value rhs);
    Value operator>(Value rhs);
    Value operator>=(Value rhs);
    Value operator<(Value rhs);
    Value operator<=(Value rhs);

    nyx::ValueType type{};
    std::any data;
};

struct Variable {
    explicit Variable() = default;

    std::string name;
    Value value;
};

struct LocalContext {
    explicit LocalContext() = default;

    std::vector<Variable*> vars;
};
struct GlobalContext : public LocalContext {
    explicit GlobalContext();

    std::vector<Function*> funcs;
    std::vector<Statement*> stmts;
    std::unordered_map<std::string,
                       Value (*)(GlobalContext*, std::vector<Value>)>
        builtin;
};
}  // namespace nyx
