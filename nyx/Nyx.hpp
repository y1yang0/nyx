#pragma once

#include <any>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

struct Statement;
struct Expression;

namespace nyx {
enum ValueType { Int, Double, String, Bool, Char, Null, Array };
enum ExecutionResultType { ExecNormal, ExecReturn, ExecBreak, ExecContinue };

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
    inline bool isType() const;

    template <typename _CastingType>
    inline _CastingType cast();
    template <typename _CastingType>
    inline _CastingType cast() const;

    template <typename _DataType>
    inline void set(_DataType data);

    Value operator+(const Value& rhs);
    Value operator-(const Value& rhs);
    Value operator*(const Value& rhs);
    Value operator/(const Value& rhs);
    Value operator%(const Value& rhs);

    Value operator&&(const Value& rhs);
    Value operator||(const Value& rhs);

    Value operator==(const Value& rhs);
    Value operator!=(const Value& rhs);
    Value operator>(const Value& rhs);
    Value operator>=(const Value& rhs);
    Value operator<(const Value& rhs);
    Value operator<=(const Value& rhs);

    Value operator&(const Value& rhs);
    Value operator|(const Value& rhs);

    nyx::ValueType type{};
    std::any data;
};

struct ExecResult {
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

    bool hasVariable(const std::string& identName);
    void createVariable(const std::string& identName, Value value);
    Variable* getVariable(const std::string& identName);

    void addFunction(const std::string& name, Function* f);
    bool hasFunction(const std::string& name);
    Function* getFunction(const std::string& name);

private:
    std::unordered_map<std::string, Variable*> vars;
    std::unordered_map<std::string, Function*> funcs;
};

class Runtime : public Context {
    using BuiltinFuncType = Value (*)(Runtime*, std::deque<Context*>,
                                      std::vector<Value>);

public:
    explicit Runtime();

    bool hasBuiltinFunction(const std::string& name);
    BuiltinFuncType getBuiltinFunction(const std::string& name);

    void addStatement(Statement* stmt);
    std::vector<Statement*>& getStatements();

private:
    std::unordered_map<std::string, BuiltinFuncType> builtin;
    std::vector<Statement*> stmts;
};

template <int _NyxType>
inline bool Value::isType() const {
    return this->type == _NyxType;
}

template <typename _CastingType>
inline _CastingType Value::cast() {
    return std::any_cast<_CastingType>(data);
}

template <typename _CastingType>
inline _CastingType Value::cast() const {
    return std::any_cast<_CastingType>(data);
}

template <typename _DataType>
inline void Value::set(_DataType data) {
    this->data = std::make_any<_DataType>(std::move(data));
}
}  // namespace nyx
