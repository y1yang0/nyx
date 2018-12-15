#pragma once

#include <stdio.h>
#include <any>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

struct Block;
struct Statement;

namespace nyx {
enum ValueType { Int, Double, String, Bool, Null };

struct Function {
    explicit Function() = default;
    ~Function() { delete block; }

    std::string name;
    std::vector<std::string> params;
    Block* block{};
};

struct Value {
    explicit Value() = default;
    explicit Value(nyx::ValueType type) : type(type) {}
    explicit Value(nyx::ValueType type, std::any data)
        : type(type), data(std::move(data)) {}

    template <int _NyxType>
    inline bool isType();

    template <typename _CastingType>
    inline _CastingType value_cast();

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

    Value operator&(Value rhs);
    Value operator|(Value rhs);

    nyx::ValueType type{};
    std::any data;
};

struct Variable {
    explicit Variable() = default;

    std::string name;
    Value value;
};

class Context {
public:
    explicit Context() = default;
    virtual ~Context();

    bool removeVariable(const std::string& identName);
    bool hasVariable(const std::string& identName);
    void addVariable(const std::string& identName, nyx::Value value);

    nyx::Variable* getVariable(const std::string& identName);

    void addFunction(const std::string& name, Function* f);
    bool hasFunction(const std::string& name);
    Function* getFunction(const std::string& name);

private:
    std::unordered_map<std::string, Variable*> vars;
    std::unordered_map<std::string, Function*> funcs;
};

class NyxContext {
public:
    explicit NyxContext();

    bool hasBuiltinFunction(const std::string& name);
    Value (*getBuiltinFunction(const std::string& name))(NyxContext*,
                                                         std::deque<Context*>,
                                                         std::vector<Value>);
    void addStatement(Statement* stmt);

    std::vector<Statement*> getStatements();

private:
    std::unordered_map<std::string, Value (*)(NyxContext*, std::deque<Context*>,
                                              std::vector<Value>)>
        builtin;
    std::vector<Statement*> stmts;
};

template <int _NyxType>
inline bool Value::isType() {
    return this->type == _NyxType;
}

template <typename _CastingType>
inline _CastingType Value::value_cast() {
    return std::any_cast<_CastingType>(data);
}
}  // namespace nyx
