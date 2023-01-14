// MIT License
//
// Copyright (c) 2023 y1yang0 <kelthuzadx@qq.com>
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

#include "Object.hpp"
#include "Builtin.h"
#include "Runtime.hpp"
#include "Utils.hpp"

bool Object::equalsDeep(Object* b) const {
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
            auto elements1 = as<std::vector<Object*>>();
            auto elements2 = b->as<std::vector<Object*>>();
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
            auto elements = as<std::vector<Object*>>();
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

Object* Object::clone() const {
    return runtime->newObject(type, data);
}

bool Object::isPrimitive() const {
    if (anyone(type, Int, Double, String, Bool, Char)) {
        return true;
    }
    return false;
}

Object* Object::operator+(Object* rhs) const {
    // Basic
    if (isType(Int) && rhs->isType(Int)) {
        int result = as<int>() + rhs->as<int>();
        return runtime->newObject(Int, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        double result = as<double>() + rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Int) && rhs->isType(Double)) {
        double result = as<int>() + rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Double) && rhs->isType(Int)) {
        double result = as<double>() + rhs->as<int>();
        return runtime->newObject(Double, result);
    } else if (isType(Char) && rhs->isType(Int)) {
        char result = static_cast<char>(as<char>() + rhs->as<int>());
        return runtime->newObject(Char, result);
    } else if (isType(Int) && rhs->isType(Char)) {
        char result = static_cast<char>(as<int>() + rhs->as<char>());
        return runtime->newObject(Char, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        char result = static_cast<char>(as<char>() + rhs->as<char>());
        return runtime->newObject(Char, result);
    }
    // String
    // One of operands has string type, we say the result value was a string
    else if (isType(String) || rhs->isType(String)) {
        std::string result = this->toString() + rhs->toString();
        return runtime->newObject(String, result);
    }
    // Array
    else if (isType(Array)) {
        auto result = this->as<std::vector<Object*>>();
        result.push_back(rhs);
        return runtime->newObject(Array, result);
    } else if (rhs->isType(Array)) {
        auto result = rhs->as<std::vector<Object*>>();
        result.push_back(const_cast<Object*>(this));
        return runtime->newObject(String, result);
    }
    // Invalid
    else {
        panic("TypeError: unexpected arguments of operator +");
    }
    return nullptr;
}

Object* Object::operator-(Object* rhs) const {
    if (isType(Int) && rhs->isType(Int)) {
        int result = as<int>() - rhs->as<int>();
        return runtime->newObject(Int, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        double result = as<double>() - rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Int) && rhs->isType(Double)) {
        double result = as<int>() - rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Double) && rhs->isType(Int)) {
        double result = as<double>() - rhs->as<int>();
        return runtime->newObject(Double, result);
    } else if (isType(Char) && rhs->isType(Int)) {
        char result = static_cast<char>(as<char>() - rhs->as<int>());
        return runtime->newObject(Char, result);
    } else if (isType(Int) && rhs->isType(Char)) {
        char result = static_cast<char>(as<int>() - rhs->as<char>());
        return runtime->newObject(Char, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        char result = static_cast<char>(as<char>() - rhs->as<char>());
        return runtime->newObject(Char, result);
    } else {
        panic("TypeError: unexpected arguments of operator -");
    }

    return nullptr;
}

Object* Object::operator*(Object* rhs) const {
    // Basic
    if (isType(Int) && rhs->isType(Int)) {
        int result = as<int>() * rhs->as<int>();
        return runtime->newObject(Int, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        double result = as<double>() * rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Int) && rhs->isType(Double)) {
        double result = as<int>() * rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Double) && rhs->isType(Int)) {
        double result = as<double>() * rhs->as<int>();
        return runtime->newObject(Double, result);
    }
    // String
    else if (isType(String) && rhs->isType(Int)) {
        std::string result = repeatString(rhs->as<int>(), as<std::string>());
        return runtime->newObject(String, result);
    } else if (isType(Int) && rhs->isType(String)) {
        std::string result = repeatString(as<int>(), rhs->as<std::string>());
        return runtime->newObject(String, result);
    }
    // Invalid
    else {
        panic("TypeError: unexpected arguments of operator *");
    }
    return nullptr;
}

Object* Object::operator/(Object* rhs) const {
    if (isType(Int) && rhs->isType(Int)) {
        int result = as<int>() / rhs->as<int>();
        return runtime->newObject(Int, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        double result = as<double>() / rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Int) && rhs->isType(Double)) {
        double result = as<int>() / rhs->as<double>();
        return runtime->newObject(Double, result);
    } else if (isType(Double) && rhs->isType(Int)) {
        double result = as<double>() / rhs->as<int>();
        return runtime->newObject(Double, result);
    } else {
        panic("TypeError: unexpected arguments of operator /");
    }
    return nullptr;
}

Object* Object::operator%(Object* rhs) const {
    checkObjectType(this, Int);
    checkObjectType(rhs, Int);
    int result = (int)as<int>() % rhs->as<int>();
    return runtime->newObject(Int, result);
}

Object* Object::operator&&(Object* rhs) const {
    checkObjectType(this, Bool);
    checkObjectType(rhs, Bool);
    bool result = (as<bool>() && rhs->as<bool>());
    return runtime->newObject(Bool, result);
}

Object* Object::operator||(Object* rhs) const {
    checkObjectType(this, Bool);
    checkObjectType(rhs, Bool);
    bool result = (as<bool>() || rhs->as<bool>());
    return runtime->newObject(Bool, result);
}

Object* Object::operator==(Object* rhs) const {
    bool result = false;
    if (isType(Int) && rhs->isType(Int)) {
        result = (as<int>() == rhs->as<int>());
        return runtime->newObject(Bool, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        result = (as<double>() == rhs->as<double>());
        return runtime->newObject(Bool, result);
    } else if (isType(String) && rhs->isType(String)) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr == rhsStr);
        return runtime->newObject(Bool, result);
    } else if (isType(Bool) && rhs->isType(Bool)) {
        result = (as<bool>() == rhs->as<bool>());
        return runtime->newObject(Bool, result);
    } else if (this->type == Null && rhs->type == Null) {
        result = true;
        return runtime->newObject(Bool, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        result = (as<char>() == rhs->as<char>());
        return runtime->newObject(Bool, result);
    } else if (isType(Array) && rhs->isType(Array)) {
        result = this->equalsDeep(rhs);
        return runtime->newObject(Bool, result);
    } else {
        panic("TypeError: unexpected arguments of operator !=");
    }
    return nullptr;
}

Object* Object::operator!=(Object* rhs) const {
    bool result = false;
    if (isType(Int) && rhs->isType(Int)) {
        result = (as<int>() != rhs->as<int>());
        return runtime->newObject(Bool, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        result = (as<double>() != rhs->as<double>());
        return runtime->newObject(Bool, result);
    } else if (isType(String) && rhs->isType(String)) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr != rhsStr);
        return runtime->newObject(Bool, result);
    } else if (isType(Bool) && rhs->isType(Bool)) {
        result = (as<bool>() != rhs->as<bool>());
        return runtime->newObject(Bool, result);
    } else if (this->type == Null && rhs->type == Null) {
        result = false;
        return runtime->newObject(Bool, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        result = (as<char>() != rhs->as<char>());
        return runtime->newObject(Bool, result);
    } else if (isType(Array) && rhs->isType(Array)) {
        result = !this->equalsDeep(rhs);
        return runtime->newObject(Bool, result);
    } else {
        panic("TypeError: unexpected arguments of operator !=");
    }
    return nullptr;
}

Object* Object::operator>(Object* rhs) const {
    bool result = false;
    if (isType(Int) && rhs->isType(Int)) {
        result = (as<int>() > rhs->as<int>());
        return runtime->newObject(Bool, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        result = (as<double>() > rhs->as<double>());
        return runtime->newObject(Bool, result);
    } else if (isType(String) && rhs->isType(String)) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr > rhsStr);
        return runtime->newObject(Bool, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        result = (as<char>() > rhs->as<char>());
        return runtime->newObject(Bool, result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator>=(Object* rhs) const {
    bool result = false;
    if (isType(Int) && rhs->isType(Int)) {
        result = (as<int>() >= rhs->as<int>());
        return runtime->newObject(Bool, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        result = (as<double>() >= rhs->as<double>());
        return runtime->newObject(Bool, result);
    } else if (isType(String) && rhs->isType(String)) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr >= rhsStr);
        return runtime->newObject(Bool, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        result = (as<char>() >= rhs->as<char>());
        return runtime->newObject(Bool, result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator<(Object* rhs) const {
    bool result = false;
    if (isType(Int) && rhs->isType(Int)) {
        result = (as<int>() < rhs->as<int>());
        return runtime->newObject(Bool, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        result = (as<double>() < rhs->as<double>());
        return runtime->newObject(Bool, result);
    } else if (isType(String) && rhs->isType(String)) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr < rhsStr);
        return runtime->newObject(Bool, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        result = (as<char>() < rhs->as<char>());
        return runtime->newObject(Bool, result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator<=(Object* rhs) const {
    bool result = false;
    if (isType(Int) && rhs->isType(Int)) {
        result = (as<int>() <= rhs->as<int>());
        return runtime->newObject(Bool, result);
    } else if (isType(Double) && rhs->isType(Double)) {
        result = (as<double>() <= rhs->as<double>());
        return runtime->newObject(Bool, result);
    } else if (isType(String) && rhs->isType(String)) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr <= rhsStr);
        return runtime->newObject(Bool, result);
    } else if (isType(Char) && rhs->isType(Char)) {
        result = (as<char>() <= rhs->as<char>());
        return runtime->newObject(Bool, result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator&(Object* rhs) const {
    checkObjectType(this, Int);
    checkObjectType(rhs, Int);

    int result = (as<int>() & rhs->as<int>());
    return runtime->newObject(Int, result);
}

Object* Object::operator|(Object* rhs) const {
    checkObjectType(this, Int);
    checkObjectType(rhs, Int);

    int result = (as<int>() | rhs->as<int>());
    return runtime->newObject(Int, result);
}

Object* Object::operator-() const {
    switch (type) {
        case Int:
            return runtime->newObject(Int, -std::any_cast<int>(data));
        case Double:
            return runtime->newObject(Double, -std::any_cast<double>(data));
        default:
            panic(
                "TypeError: invalid operand type for operator "
                "-(negative)");
    }
    return nullptr;
}

Object* Object::operator!() const {
    checkObjectType(this, Bool);
    return runtime->newObject(Bool, !std::any_cast<bool>(data));
}

Object* Object::operator~() const {
    checkObjectType(this, Int);
    return runtime->newObject(Int, ~std::any_cast<int>(data));
}