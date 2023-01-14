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

#ifndef NYX_OBJECT_HPP
#define NYX_OBJECT_HPP

enum ValueType { Int, Double, String, Bool, Char, Null, Array, Closure };

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "Runtime.hpp"

struct Statement;
struct Context;

//===----------------------------------------------------------------------===//
// Runtime object
//===----------------------------------------------------------------------===//
class Object {
    friend class Runtime;

public:
    int asInt() const { return *(int*)(data); }
    double asDouble() const { return *(double*)(data); }
    std::string asString() const { return *(std::string*)(data); }
    bool asBool() const { return *(bool*)(data); }
    char asChar() const { return *(char*)(data); }
    std::nullptr_t asNull() const { return nullptr; }
    ObjectArray asArray() const { return *(ObjectArray*)(data); }
    Func asClosure() const { return *(Func*)(data); }

    inline bool isType(ValueType t) const { return t == type; }

    Object* operator+(Object* rhs) const;

    Object* operator-(Object* rhs) const;

    Object* operator*(Object* rhs) const;

    Object* operator/(Object* rhs) const;

    Object* operator%(Object* rhs) const;

    Object* operator&&(Object* rhs) const;

    Object* operator||(Object* rhs) const;

    Object* operator==(Object* rhs) const;

    Object* operator!=(Object* rhs) const;

    Object* operator>(Object* rhs) const;

    Object* operator>=(Object* rhs) const;

    Object* operator<(Object* rhs) const;

    Object* operator<=(Object* rhs) const;

    Object* operator&(Object* rhs) const;

    Object* operator|(Object* rhs) const;

    Object* operator-() const;

    Object* operator!() const;

    Object* operator~() const;

    bool equalsDeep(Object* b) const;

    std::string toString() const;

    bool isPrimitive() const;

    ValueType getType() const { return type; }

    template <typename T>
    void resetObject(T data) {
        *(T*)(this->data) = data;
    }

private:
    explicit Object() = default;

    explicit Object(ValueType type, void* data) : type(type), data(data) {}

    ValueType type;
    void* data;
};

#endif  // NYX_OBJECT_HPP
