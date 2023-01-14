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
            return asBool() == b->asBool();
        case Double:
            return asDouble() == b->asDouble();
        case Int:
            return asInt() == b->asInt();
        case Null:
            return true;
        case String:
            return asString() == b->asString();
        case Char:
            return asChar() == b->asChar();
        case Array: {
            auto elements1 = asArray();
            auto elements2 = b->asArray();
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
            return asBool() ? "true" : "false";
        case Double:
            return std::to_string(asDouble());
        case Int:
            return std::to_string(asInt());
        case Null:
            return "null";
        case String:
            return asString();
        case Char: {
            std::string str;
            str += asChar();
            return str;
        }
        case Array: {
            std::string str = "[";
            auto elements = asArray();
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

bool Object::isPrimitive() const {
    if (anyone(type, Int, Double, String, Bool, Char)) {
        return true;
    }
    return false;
}

Object* Object::operator+(Object* rhs) const {
    // Basic
    if (isInt() && rhs->isInt()) {
        int result = asInt() + rhs->asInt();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        double result = asDouble() + rhs->asDouble();
        return runtime->newObject(result);
    } else if (isInt() && rhs->isDouble()) {
        double result = asInt() + rhs->asDouble();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isInt()) {
        double result = asDouble() + rhs->asInt();
        return runtime->newObject(result);
    } else if (isChar() && rhs->isInt()) {
        char result = static_cast<char>(asChar() + rhs->asInt());
        return runtime->newObject(result);
    } else if (isInt() && rhs->isChar()) {
        char result = static_cast<char>(asInt() + rhs->asChar());
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        char result = static_cast<char>(asChar() + rhs->asChar());
        return runtime->newObject(result);
    }
    // String
    // One of operands has string type, we say the result value was a string
    else if (isString() || rhs->isString()) {
        std::string result = this->toString() + rhs->toString();
        return runtime->newObject(result);
    }
    // Array
    else if (isArray()) {
        auto result = this->asArray();
        result.push_back(rhs);
        return runtime->newObject(result);
    } else if (rhs->isArray()) {
        auto result = rhs->asArray();
        result.push_back(const_cast<Object*>(this));
        return runtime->newObject(result);
    }
    // Invalid
    else {
        panic("TypeError: unexpected arguments of operator +");
    }
    return nullptr;
}

Object* Object::operator-(Object* rhs) const {
    if (isInt() && rhs->isInt()) {
        int result = asInt() - rhs->asInt();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        double result = asDouble() - rhs->asDouble();
        return runtime->newObject(result);
    } else if (isInt() && rhs->isDouble()) {
        double result = asInt() - rhs->asDouble();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isInt()) {
        double result = asDouble() - rhs->asInt();
        return runtime->newObject(result);
    } else if (isChar() && rhs->isInt()) {
        char result = static_cast<char>(asChar() - rhs->asInt());
        return runtime->newObject(result);
    } else if (isInt() && rhs->isChar()) {
        char result = static_cast<char>(asInt() - rhs->asChar());
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        char result = static_cast<char>(asChar() - rhs->asChar());
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator -");
    }

    return nullptr;
}

Object* Object::operator*(Object* rhs) const {
    // Basic
    if (isInt() && rhs->isInt()) {
        int result = asInt() * rhs->asInt();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        double result = asDouble() * rhs->asDouble();
        return runtime->newObject(result);
    } else if (isInt() && rhs->isDouble()) {
        double result = asInt() * rhs->asDouble();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isInt()) {
        double result = asDouble() * rhs->asInt();
        return runtime->newObject(result);
    }
    // String
    else if (isString() && rhs->isInt()) {
        std::string result = repeatString(rhs->asInt(), asString());
        return runtime->newObject(result);
    } else if (isInt() && rhs->isString()) {
        std::string result = repeatString(asInt(), rhs->asString());
        return runtime->newObject(result);
    }
    // Invalid
    else {
        panic("TypeError: unexpected arguments of operator *");
    }
    return nullptr;
}

Object* Object::operator/(Object* rhs) const {
    if (isInt() && rhs->isInt()) {
        int result = asInt() / rhs->asInt();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        double result = asDouble() / rhs->asDouble();
        return runtime->newObject(result);
    } else if (isInt() && rhs->isDouble()) {
        double result = asInt() / rhs->asDouble();
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isInt()) {
        double result = asDouble() / rhs->asInt();
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator /");
    }
    return nullptr;
}

Object* Object::operator%(Object* rhs) const {
    checkObjectType(this, Int);
    checkObjectType(rhs, Int);
    int result = (int)asInt() % rhs->asInt();
    return runtime->newObject(result);
}

Object* Object::operator&&(Object* rhs) const {
    checkObjectType(this, Bool);
    checkObjectType(rhs, Bool);
    bool result = (asBool() && rhs->asBool());
    return runtime->newObject(result);
}

Object* Object::operator||(Object* rhs) const {
    checkObjectType(this, Bool);
    checkObjectType(rhs, Bool);
    bool result = (asBool() || rhs->asBool());
    return runtime->newObject(result);
}

Object* Object::operator==(Object* rhs) const {
    bool result = false;
    if (isInt() && rhs->isInt()) {
        result = (asInt() == rhs->asInt());
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        result = (asDouble() == rhs->asDouble());
        return runtime->newObject(result);
    } else if (isString() && rhs->isString()) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr == rhsStr);
        return runtime->newObject(result);
    } else if (isBool() && rhs->isBool()) {
        result = (asBool() == rhs->asBool());
        return runtime->newObject(result);
    } else if (this->type == Null && rhs->type == Null) {
        result = true;
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        result = (asChar() == rhs->asChar());
        return runtime->newObject(result);
    } else if (isArray() && rhs->isArray()) {
        result = this->equalsDeep(rhs);
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator !=");
    }
    return nullptr;
}

Object* Object::operator!=(Object* rhs) const {
    bool result = false;
    if (isInt() && rhs->isInt()) {
        result = (asInt() != rhs->asInt());
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        result = (asDouble() != rhs->asDouble());
        return runtime->newObject(result);
    } else if (isString() && rhs->isString()) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr != rhsStr);
        return runtime->newObject(result);
    } else if (isBool() && rhs->isBool()) {
        result = (asBool() != rhs->asBool());
        return runtime->newObject(result);
    } else if (this->type == Null && rhs->type == Null) {
        result = false;
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        result = (asChar() != rhs->asChar());
        return runtime->newObject(result);
    } else if (isArray() && rhs->isArray()) {
        result = !this->equalsDeep(rhs);
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator !=");
    }
    return nullptr;
}

Object* Object::operator>(Object* rhs) const {
    bool result = false;
    if (isInt() && rhs->isInt()) {
        result = (asInt() > rhs->asInt());
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        result = (asDouble() > rhs->asDouble());
        return runtime->newObject(result);
    } else if (isString() && rhs->isString()) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr > rhsStr);
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        result = (asChar() > rhs->asChar());
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator>=(Object* rhs) const {
    bool result = false;
    if (isInt() && rhs->isInt()) {
        result = (asInt() >= rhs->asInt());
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        result = (asDouble() >= rhs->asDouble());
        return runtime->newObject(result);
    } else if (isString() && rhs->isString()) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr >= rhsStr);
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        result = (asChar() >= rhs->asChar());
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator<(Object* rhs) const {
    bool result = false;
    if (isInt() && rhs->isInt()) {
        result = (asInt() < rhs->asInt());
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        result = (asDouble() < rhs->asDouble());
        return runtime->newObject(result);
    } else if (isString() && rhs->isString()) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr < rhsStr);
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        result = (asChar() < rhs->asChar());
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator<=(Object* rhs) const {
    bool result = false;
    if (isInt() && rhs->isInt()) {
        result = (asInt() <= rhs->asInt());
        return runtime->newObject(result);
    } else if (isDouble() && rhs->isDouble()) {
        result = (asDouble() <= rhs->asDouble());
        return runtime->newObject(result);
    } else if (isString() && rhs->isString()) {
        std::string lhsStr, rhsStr;
        lhsStr = this->toString();
        rhsStr = rhs->toString();
        result = (lhsStr <= rhsStr);
        return runtime->newObject(result);
    } else if (isChar() && rhs->isChar()) {
        result = (asChar() <= rhs->asChar());
        return runtime->newObject(result);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return nullptr;
}

Object* Object::operator&(Object* rhs) const {
    checkObjectType(this, Int);
    checkObjectType(rhs, Int);

    int result = (asInt() & rhs->asInt());
    return runtime->newObject(result);
}

Object* Object::operator|(Object* rhs) const {
    checkObjectType(this, Int);
    checkObjectType(rhs, Int);

    int result = (asInt() | rhs->asInt());
    return runtime->newObject(result);
}

Object* Object::operator-() const {
    switch (type) {
        case Int:
            return runtime->newObject(-(*(int*)data));
        case Double:
            return runtime->newObject(-(*(double*)data));
        default:
            panic("TypeError: invalid operand type for operator -(negative)");
    }
    return nullptr;
}

Object* Object::operator!() const {
    checkObjectType(this, Bool);
    return runtime->newObject(!(*(bool*)data));
}

Object* Object::operator~() const {
    checkObjectType(this, Int);
    return runtime->newObject(-(*(int*)data));
}