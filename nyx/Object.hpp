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

#include <any>
#include <deque>
#include <vector>
#include "Runtime.hpp"

struct Statement;
struct Context;

class Object {
    friend class Runtime;

public:
    inline bool isType(ValueType t) const { return t == type; }

    template <typename _CastingType>
    inline _CastingType as();

    template <typename _CastingType>
    inline _CastingType as() const;

    template <typename _DataType>
    inline void set(_DataType data);

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

    Object* clone() const;

    bool isPrimitive();

    ValueType getType() const { return type; }

private:
    explicit Object() = default;

    explicit Object(ValueType type, std::any data)
        : type(type), data(std::move(data)) {}

    ValueType type;
    std::any data;
};

template <typename _CastingType>
inline _CastingType Object::as() {
    return std::any_cast<_CastingType>(data);
}

template <typename _CastingType>
inline _CastingType Object::as() const {
    return std::any_cast<_CastingType>(data);
}

template <typename _DataType>
inline void Object::set(_DataType data) {
    this->data = std::make_any<_DataType>(std::move(data));
}

#include <any>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#endif  // NYX_OBJECT_HPP
