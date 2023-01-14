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

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

struct Statement;
struct Expression;
struct Context;
class Object;

using ObjectArray = std::vector<Object*>;
using ContextChain = std::deque<Context*>;

enum ExecutionResultType { ExecNormal, ExecReturn, ExecBreak, ExecContinue };

struct Block {
    explicit Block() = default;

    std::vector<Statement*> stmts;
};

struct Func {
    explicit Func() = default;

    std::string name;
    ContextChain* outerContext{};
    std::vector<std::string> params;
    Block* block{};
};

struct ExecResult {
    explicit ExecResult(ExecutionResultType execType)
        : execType(execType), retValue(nullptr) {}

    explicit ExecResult(ExecutionResultType execType, Object* retValue)
        : execType(execType), retValue(retValue) {}

    ExecutionResultType execType;
    Object* retValue;
};

struct Variable {
    explicit Variable() = default;

    std::string name;
    Object* value;
};

class Context {
public:
    explicit Context() = default;

    virtual ~Context();

    bool hasVariable(const std::string& identName);

    void createVariable(const std::string& identName, Object* value);

    Variable* getVariable(const std::string& identName);

    void addFunction(const std::string& name, Func* f);

    bool hasFunction(const std::string& name);

    Func* getFunction(const std::string& name);

private:
    std::unordered_map<std::string, Variable*> vars;
    std::unordered_map<std::string, Func*> funcs;
};

class Runtime : public Context {
    using BuiltinFuncType = Object* (*)(Runtime*, ContextChain*, ObjectArray);

public:
    explicit Runtime();

    bool hasBuiltinFunction(const std::string& name);

    BuiltinFuncType getBuiltinFunction(const std::string& name);

    void addStatement(Statement* stmt);

    std::vector<Statement*>& getStatements();

    Object* newObject(int data);
    Object* newObject(double data);
    Object* newObject(std::string data);
    Object* newObject(bool data);
    Object* newObject(char c);
    Object* newObject(ObjectArray data);
    Object* newObject(Func data);
    Object* newObject();
    Object* cloneObject(Object* object);

    template <typename T>
    void resetObject(Object* object, T data);

private:
    std::unordered_map<std::string, BuiltinFuncType> builtin;
    std::vector<Statement*> stmts;
    // TODO: create object in managed heap and support GC to make it a "real
    // heap"
    ObjectArray heap;
};

extern Runtime* runtime;