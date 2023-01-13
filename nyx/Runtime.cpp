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

#include "Builtin.h"
#include "Runtime.hpp"
#include "Utils.hpp"

Context::~Context() {
    for (const auto &v: vars) {
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
}

bool Runtime::hasBuiltinFunction(const std::string &name) {
    return builtin.count(name) == 1;
}

Runtime::BuiltinFuncType Runtime::getBuiltinFunction(const std::string &name) {
    if (auto res = builtin.find(name); res != builtin.end()) {
        return res->second;
    }
    return nullptr;
}

void Runtime::addStatement(Statement *stmt) { stmts.push_back(stmt); }

std::vector<Statement *> &Runtime::getStatements() { return stmts; }

Object *Runtime::newObject(ValueType type, std::any data) {
    // TODO: create object in managed heap and support GC
    return new Object(type, std::move(data));
}

Object *Runtime::newNullObject() {
    // TODO: create object in managed heap and support GC
    return new Object(Null, nullptr);
}

bool Context::hasVariable(const std::string &identName) {
    return vars.count(identName) == 1;
}

void Context::createVariable(const std::string &identName, Object *value) {
    auto *var = new Variable();
    var->name = identName;
    var->value = value;
    vars.emplace(identName, var);
}

Variable *Context::getVariable(const std::string &identName) {
    if (auto res = vars.find(identName); res != vars.end()) {
        return res->second;
    }
    return nullptr;
}

void Context::addFunction(const std::string &name, Function *f) {
    funcs.insert(std::make_pair(name, f));
}

bool Context::hasFunction(const std::string &name) {
    return funcs.count(name) == 1;
}

Function *Context::getFunction(const std::string &name) {
    if (auto f = funcs.find(name); f != funcs.end()) {
        return f->second;
    }
    return nullptr;
}


bool Object::equalsDeep(Object *b) {
    if (type != b->type) {
        return false;
    }
    switch (type) {
        case Bool:
            return as<bool>() == b->as<bool>();
        case Double:
            return as<double>() == b->as<double>();
        case Int:
            return as<int>() == b->as<int>();
        case Null:
            return true;
        case String:
            return as<std::string>() == b->as<std::string>();
        case Char:
            return as<char>() == b->as<char>();
        case Array: {
            auto elements1 = as<std::vector<Object *>>();
            auto elements2 = b->as<std::vector<Object *>>();
            if (elements1.size() != elements2.size()) {
                return false;
            }
            for (int i = 0; i < elements1.size(); i++) {
                if (!elements1[i]->equalsDeep(elements2[i])) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

std::string Object::toString() const {
    switch (type) {
        case Bool:
            return as<bool>() ? "true" : "false";
        case Double:
            return std::to_string(as<double>());
        case Int:
            return std::to_string(as<int>());
        case Null:
            return "null";
        case String:
            return as<std::string>();
        case Char: {
            std::string str;
            str += as<char>();
            return str;
        }
        case Array: {
            std::string str = "[";
            auto elements = as<std::vector<Object *>>();
            for (int i = 0; i < elements.size(); i++) {
                str += elements[i]->toString();

                if (i != elements.size() - 1) {
                    str += ",";
                }
            }
            str += "]";
            return str;
        }
        case Closure: {
            return "closure";
        }
    }
    return "<unknown>";
}

Object *Object::clone() const {
    Object *n = new Object();
    n->type = this->type;
    n->data = this->data;
    return n;
}

bool Object::isPrimitive() {
    if (anyone(type, Int, Double, String, Bool, Char)) {
        return true;
    }
    return false;
}

Object *Object::operator+(Object *rhs) const {
    Object *result = new Object();
    // Basic
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Int;
        result->data = as<int>() + rhs->as<int>();
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<double>() + rhs->as<double>();
    } else if (isType<Int>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<int>() + rhs->as<double>();
    } else if (isType<Double>() && rhs->isType<Int>()) {
        result->type = Double;
        result->data = as<double>() + rhs->as<int>();
    } else if (isType<Char>() && rhs->isType<Int>()) {
        result->type = Char;
        result->data = static_cast<char>(as<char>() + rhs->as<int>());
    } else if (isType<Int>() && rhs->isType<Char>()) {
        result->type = Char;
        result->data = static_cast<char>(as<int>() + rhs->as<char>());
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Char;
        result->data = static_cast<char>(as<char>() + rhs->as<char>());
    }
        // String
        // One of operands has string type, we say the result value was a string
    else if (isType<String>() || rhs->isType<String>()) {
        result->type = String;
        result->data = this->toString() + rhs->toString();
    }
        // Array
    else if (isType<Array>()) {
        result->type = Array;
        auto resultArr = this->as<std::vector<Object *>>();
        resultArr.push_back(rhs);
        result->data = resultArr;
    } else if (rhs->isType<Array>()) {
        result->type = Array;
        auto resultArr = rhs->as<std::vector<Object *>>();
        resultArr.push_back(const_cast<Object *>(this));
        result->data = resultArr;
    }
        // Invalid
    else {
        panic("TypeError: unexpected arguments of operator +");
    }
    return result;
}

Object *Object::operator-(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Int;
        result->data = as<int>() - rhs->as<int>();
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<double>() - rhs->as<double>();
    } else if (isType<Int>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<int>() - rhs->as<double>();
    } else if (isType<Double>() && rhs->isType<Int>()) {
        result->type = Double;
        result->data = as<double>() - rhs->as<int>();
    } else if (isType<Char>() && rhs->isType<Int>()) {
        result->type = Char;
        result->data = static_cast<char>(as<char>() - rhs->as<int>());
    } else if (isType<Int>() && rhs->isType<Char>()) {
        result->type = Char;
        result->data = static_cast<char>(as<int>() - rhs->as<char>());
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Char;
        result->data = static_cast<char>(as<char>() - rhs->as<char>());
    } else {
        panic("TypeError: unexpected arguments of operator -");
    }

    return result;
}

Object *Object::operator*(Object *rhs) const {
    Object *result = new Object();
    // Basic
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Int;
        result->data = as<int>() * rhs->as<int>();
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<double>() * rhs->as<double>();
    } else if (isType<Int>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<int>() * rhs->as<double>();
    } else if (isType<Double>() && rhs->isType<Int>()) {
        result->type = Double;
        result->data = as<double>() * rhs->as<int>();
    }
        // String
    else if (isType<String>() && rhs->isType<Int>()) {
        result->type = String;
        result->data = repeatString(rhs->as<int>(), as<std::string>());
    } else if (isType<Int>() && rhs->isType<String>()) {
        result->type = String;
        result->data = repeatString(as<int>(), rhs->as<std::string>());
    }
        // Invalid
    else {
        panic("TypeError: unexpected arguments of operator *");
    }
    return result;
}

Object *Object::operator/(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Int;
        result->data = as<int>() / rhs->as<int>();
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<double>() / rhs->as<double>();
    } else if (isType<Int>() && rhs->isType<Double>()) {
        result->type = Double;
        result->data = as<int>() / rhs->as<double>();
    } else if (isType<Double>() && rhs->isType<Int>()) {
        result->type = Double;
        result->data = as<double>() / rhs->as<int>();
    } else {
        panic("TypeError: unexpected arguments of operator /");
    }
    return result;
}

Object *Object::operator%(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Int;
        result->data = (int) as<int>() % rhs->as<int>();
    } else {
        panic("TypeError: unexpected arguments of operator %");
    }
    return result;
}

Object *Object::operator&&(Object *rhs) const {
    Object *result = new Object();
    if (isType<Bool>() && rhs->isType<Bool>()) {
        result->type = Bool;
        result->data = (as<bool>() && rhs->as<bool>());
    } else {
        panic("TypeError: unexpected arguments of operator &&");
    }
    return result;
}

Object *Object::operator||(Object *rhs) const {
    Object *result = new Object();
    if (isType<Bool>() && rhs->isType<Bool>()) {
        result->type = Bool;
        result->data = (as<bool>() || rhs->as<bool>());
    } else {
        panic("TypeError: unexpected arguments of operator ||");
    }
    return result;
}

Object *Object::operator==(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Bool;
        result->data = (as<int>() == rhs->as<int>());
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Bool;
        result->data = (as<double>() == rhs->as<double>());
    } else if (isType<String>() && rhs->isType<String>()) {
        result->type = Bool;
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result->data = (lhsStr == rhsStr);
    } else if (isType<Bool>() && rhs->isType<Bool>()) {
        result->type = Bool;
        result->data = (as<bool>() == rhs->as<bool>());
    } else if (this->type == Null && rhs->type == Null) {
        result->type = Bool;
        result->data = std::make_any<bool>(true);
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Bool;
        result->data = (as<char>() == rhs->as<char>());
    } else {
        panic("TypeError: unexpected arguments of operator ==");
    }
    return result;
}

Object *Object::operator!=(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Bool;
        result->data = (as<int>() != rhs->as<int>());
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Bool;
        result->data = (as<double>() != rhs->as<double>());
    } else if (isType<String>() && rhs->isType<String>()) {
        result->type = Bool;
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result->data = (lhsStr != rhsStr);
    } else if (isType<Bool>() && rhs->isType<Bool>()) {
        result->type = Bool;
        result->data = (as<bool>() != rhs->as<bool>());
    } else if (this->type == Null && rhs->type == Null) {
        result->type = Bool;
        result->data = std::make_any<bool>(false);
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Bool;
        result->data = (as<char>() != rhs->as<char>());
    } else {
        panic("TypeError: unexpected arguments of operator !=");
    }
    return result;
}

Object *Object::operator>(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Bool;
        result->data = (as<int>() > rhs->as<int>());
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Bool;
        result->data = (as<double>() > rhs->as<double>());
    } else if (isType<String>() && rhs->isType<String>()) {
        result->type = Bool;
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result->data = (lhsStr > rhsStr);
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Bool;
        result->data = (as<char>() > rhs->as<char>());
    } else {
        panic("TypeError: unexpected arguments of operator >");
    }
    return result;
}

Object *Object::operator>=(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Bool;
        result->data = (as<int>() >= rhs->as<int>());
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Bool;
        result->data = (as<double>() >= rhs->as<double>());
    } else if (isType<String>() && rhs->isType<String>()) {
        result->type = Bool;
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result->data = (lhsStr >= rhsStr);
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Bool;
        result->data = (as<char>() >= rhs->as<char>());
    } else {
        panic("TypeError: unexpected arguments of operator >=");
    }
    return result;
}

Object *Object::operator<(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Bool;
        result->data = (as<int>() < rhs->as<int>());
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Bool;
        result->data = (as<double>() < rhs->as<double>());
    } else if (isType<String>() && rhs->isType<String>()) {
        result->type = Bool;
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result->data = (lhsStr < rhsStr);
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Bool;
        result->data = (as<char>() < rhs->as<char>());
    } else {
        panic("TypeError: unexpected arguments of operator <");
    }
    return result;
}

Object *Object::operator<=(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Bool;
        result->data = (as<int>() <= rhs->as<int>());
    } else if (isType<Double>() && rhs->isType<Double>()) {
        result->type = Bool;
        result->data = (as<double>() <= rhs->as<double>());
    } else if (isType<String>() && rhs->isType<String>()) {
        result->type = Bool;
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result->data = (lhsStr <= rhsStr);
    } else if (isType<Char>() && rhs->isType<Char>()) {
        result->type = Bool;
        result->data = (as<char>() <= rhs->as<char>());
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return result;
}

Object *Object::operator&(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Int;
        result->data = (as<int>() & rhs->as<int>());
    } else {
        panic("TypeError: unexpected arguments of operator &");
    }
    return result;
}

Object *Object::operator|(Object *rhs) const {
    Object *result = new Object();
    if (isType<Int>() && rhs->isType<Int>()) {
        result->type = Int;
        result->data = (as<int>() | rhs->as<int>());
    } else {
        panic("TypeError: unexpected arguments of operator |");
    }
    return result;
}

Object *Object::operator-() const {
    switch (type) {
        case Int:
            return new Object(Int, -std::any_cast<int>(data));
        case Double:
            return new Object(Double, -std::any_cast<double>(data));
        default:
            panic("TypeError: invalid operand type for operator "
                  "-(negative)");
    }
    return nullptr;
}

Object *Object::operator!() const {
    if (type == Bool) {
        return new Object(Bool, !std::any_cast<bool>(data));
    } else {
        panic("TypeError: invalid operand type for operator "
              "!(logical not)");
    }
    return nullptr;
}

Object *Object::operator~() const {
    if (type == Int) {
        return new Object(Int, ~std::any_cast<int>(data));
    } else {
        panic("TypeError: invalid operand type for operator "
              "~(bit not)");
    }
    return nullptr;
}
