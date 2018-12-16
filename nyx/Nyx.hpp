#pragma once

#include <any>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

struct Statement;

namespace nyx {
enum ValueType { Int, Double, String, Bool, Null };
enum ExecutionResultType { ExecNormal, ExecReturn };

struct Block {
    explicit Block() = default;

    std::vector<Statement*> stmts;
};

struct Function {
    explicit Function() = default;
    ~Function() { delete block; }

    std::string name;
    std::vector<std::string> params;
    Block* block{};
    Expression* retExpr{};
};

struct Value {
    explicit Value() {}
    explicit Value(nyx::ValueType type) : type(type) {}
    explicit Value(nyx::ValueType type, std::any data)
        : type(type), data(std::move(data)) {}

    template <int _NyxType>
    inline bool isType();

    template <typename _CastingType>
    inline _CastingType cast();

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

struct ExecResult {
    explicit ExecResult() : execType(ExecNormal) {}
    explicit ExecResult(ExecutionResultType execType) : execType(execType) {}
    explicit ExecResult(ExecutionResultType execType, Value retValue)
        : execType(execType), retValue(retValue) {}

    ExecutionResultType execType;
    Value retValue;
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
    void addVariable(const std::string& identName, Value value);
    Variable* getVariable(const std::string& identName);

    void addFunction(const std::string& name, Function* f);
    bool hasFunction(const std::string& name);
    Function* getFunction(const std::string& name);

private:
    std::unordered_map<std::string, Variable*> vars;
    std::unordered_map<std::string, Function*> funcs;
};

class Runtime {
    using BuiltinFuncType = Value (*)(Runtime*, std::deque<Context*>,
                                      std::vector<Value>);

public:
    explicit Runtime();

    bool hasBuiltinFunction(const std::string& name);
    BuiltinFuncType getBuiltinFunction(const std::string& name);

    void addStatement(Statement* stmt);
    std::vector<Statement*> getStatements();

private:
    std::unordered_map<std::string, BuiltinFuncType> builtin;
    std::vector<Statement*> stmts;
};

template <int _NyxType>
inline bool Value::isType() {
    return this->type == _NyxType;
}

template <typename _CastingType>
inline _CastingType Value::cast() {
    return std::any_cast<_CastingType>(data);
}
}  // namespace nyx
