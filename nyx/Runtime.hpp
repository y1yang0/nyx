// MIT License
//
// Copyright (c) 2018-2023 y1yang0 <kelthuzadx@qq.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <any>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

struct Statement;
struct Expression;

struct Context;

enum ValueType {
    Int, Double, String, Bool, Char, Null, Array, Closure
};

enum ExecutionResultType {
    ExecNormal, ExecReturn, ExecBreak, ExecContinue
};

struct Block {
    explicit Block() = default;

    std::vector<Statement *> stmts;
};

struct Function {
    explicit Function() = default;

    std::string name;
    std::deque<Context *> *outerContext{};
    std::vector<std::string> params;
    Block *block{};
};

class Object {
    friend class Runtime;

public:
    template<int _NyxType>
    inline bool isType() const;

    template<typename _CastingType>
    inline _CastingType as();

    template<typename _CastingType>
    inline _CastingType as() const;

    template<typename _DataType>
    inline void set(_DataType data);

    Object *operator+(Object *rhs) const;

    Object *operator-(Object *rhs) const;

    Object *operator*(Object *rhs) const;

    Object *operator/(Object *rhs) const;

    Object *operator%(Object *rhs) const;

    Object *operator&&(Object *rhs) const;

    Object *operator||(Object *rhs) const;

    Object *operator==(Object *rhs) const;

    Object *operator!=(Object *rhs) const;

    Object *operator>(Object *rhs) const;

    Object *operator>=(Object *rhs) const;

    Object *operator<(Object *rhs) const;

    Object *operator<=(Object *rhs) const;

    Object *operator&(Object *rhs) const;

    Object *operator|(Object *rhs) const;

    Object *operator-() const;

    Object *operator!() const;

    Object *operator~() const;

    bool equalsDeep(Object *b);

    std::string toString() const;

    Object *clone() const;

    bool isPrimitive();

    ValueType getType() const { return type; }

private:
    explicit Object() = default;

    explicit Object(ValueType type) : type(type) {}

    explicit Object(ValueType type, std::any data)
            : type(type), data(std::move(data)) {}

    ValueType type;
    std::any data;
};

struct ExecResult {
    explicit ExecResult(ExecutionResultType execType) : execType(execType) {}

    explicit ExecResult(ExecutionResultType execType, Object *retValue)
            : execType(execType), retValue(retValue) {}

    ExecutionResultType execType;
    Object *retValue;
};

struct Variable {
    explicit Variable() = default;

    std::string name;
    Object *value;
};

class Context {
public:
    explicit Context() = default;

    virtual ~Context();

    bool hasVariable(const std::string &identName);

    void createVariable(const std::string &identName, Object *value);

    Variable *getVariable(const std::string &identName);

    void addFunction(const std::string &name, Function *f);

    bool hasFunction(const std::string &name);

    Function *getFunction(const std::string &name);

private:
    std::unordered_map<std::string, Variable *> vars;
    std::unordered_map<std::string, Function *> funcs;
};

class Runtime : public Context {
    using BuiltinFuncType = Object *(*)(Runtime *, std::deque<Context *> *,
                                        std::vector<Object *>);

public:
    explicit Runtime();

    bool hasBuiltinFunction(const std::string &name);

    BuiltinFuncType getBuiltinFunction(const std::string &name);

    void addStatement(Statement *stmt);

    std::vector<Statement *> &getStatements();

    Object *newObject(ValueType type, std::any data);

    Object *newNullObject();

private:
    std::unordered_map<std::string, BuiltinFuncType> builtin;
    std::vector<Statement *> stmts;
};

template<int _NyxType>
inline bool Object::isType() const {
    return this->type == _NyxType;
}

template<typename _CastingType>
inline _CastingType Object::as() {
    return std::any_cast<_CastingType>(data);
}

template<typename _CastingType>
inline _CastingType Object::as() const {
    return std::any_cast<_CastingType>(data);
}

template<typename _DataType>
inline void Object::set(_DataType data) {
    this->data = std::make_any<_DataType>(std::move(data));
}

