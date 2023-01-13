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
#include "Object.hpp"

struct Statement;
struct Expression;

struct Context;

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
