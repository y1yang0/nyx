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

#include "Runtime.hpp"

#include <utility>
#include "Builtin.h"
#include "Object.hpp"
#include "Utils.hpp"

Runtime* runtime = new Runtime();

Context::~Context() {
    for (const auto& v : vars) {
        delete v.second;
    }
}

Runtime::Runtime() {
    builtin["print"] = &nyx_builtin_print;
    builtin["println"] = &nyx_builtin_println;
    builtin["typeof"] = &nyx_builtin_typeof;
    builtin["input"] = &nyx_builtin_input;
    builtin["length"] = &nyx_builtin_length;
    builtin["to_int"] = &nyx_builtin_to_int;
    builtin["to_double"] = &nyx_builtin_to_double;
    builtin["range"] = &nyx_builtin_range;
    builtin["assert"] = &nyx_builtin_assert;
    builtin["dump_ast"] = &nyx_builtin_dump_ast;
}

bool Runtime::hasBuiltinFunction(const std::string& name) {
    return builtin.count(name) == 1;
}

Runtime::BuiltinFuncType Runtime::getBuiltinFunction(const std::string& name) {
    if (auto res = builtin.find(name); res != builtin.end()) {
        return res->second;
    }
    return nullptr;
}

void Runtime::addStatement(Statement* stmt) {
    stmts.push_back(stmt);
}

std::vector<Statement*>& Runtime::getStatements() {
    return stmts;
}

Object* Runtime::newObject() {
    auto* object = new Object(Null, nullptr);
    heap.push_back(object);
    return object;
}

Object* Runtime::newObject(int data) {
    int* mem = new int;
    *mem = data;
    auto* object = new Object(Int, mem);
    heap.push_back(object);
    return object;
}
Object* Runtime::newObject(double data) {
    auto* mem = new double;
    *mem = data;
    auto* object = new Object(Double, mem);
    heap.push_back(object);
    return object;
}

Object* Runtime::newObject(std::string data) {
    auto* mem = new std::string;
    *mem = std::move(data);
    auto* object = new Object(String, mem);
    heap.push_back(object);
    return object;
}

Object* Runtime::newObject(bool data) {
    bool* mem = new bool;
    *mem = data;
    auto* object = new Object(Bool, mem);
    heap.push_back(object);
    return object;
}

Object* Runtime::newObject(char data) {
    char* mem = new char;
    *mem = data;
    auto* object = new Object(Char, mem);
    heap.push_back(object);
    return object;
}
Object* Runtime::newObject(ObjectArray data) {
    auto* mem = new ObjectArray;
    *mem = data;
    auto* object = new Object(Array, mem);
    heap.push_back(object);
    return object;
}
Object* Runtime::newObject(Func data) {
    auto* mem = new Func;
    mem[0] = data;
    auto* object = new Object(Closure, mem);
    heap.push_back(object);
    return object;
}
Object* Runtime::cloneObject(Object* object) {
    switch (object->getType()) {
        case Int:
            return newObject(object->asInt());
        case Double:
            return newObject(object->asDouble());
        case String:
            return newObject(object->asString());
        case Bool:
            return newObject(object->asBool());
        case Char:
            return newObject(object->asChar());
        case Array:
            return newObject(object->asArray());
        case Closure:
            return newObject(object->asClosure());
        default:
            panic("TypeError: unknown object type (%p)", object->type,
                  object->data);
    }
}

bool Context::hasVariable(const std::string& identName) {
    return vars.count(identName) == 1;
}

void Context::createVariable(const std::string& identName, Object* value) {
    auto* var = new Variable();
    var->name = identName;
    var->value = value;
    vars.emplace(identName, var);
}

Variable* Context::getVariable(const std::string& identName) {
    if (auto res = vars.find(identName); res != vars.end()) {
        return res->second;
    }
    return nullptr;
}

void Context::addFunction(const std::string& name, Func* f) {
    funcs.insert(std::make_pair(name, f));
}

bool Context::hasFunction(const std::string& name) {
    return funcs.count(name) == 1;
}

Func* Context::getFunction(const std::string& name) {
    if (auto f = funcs.find(name); f != funcs.end()) {
        return f->second;
    }
    return nullptr;
}

template <typename T>
void Runtime::resetObject(Object* object, T data) {
    *(T*)(object->data) = data;
}
