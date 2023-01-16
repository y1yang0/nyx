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

#include "Utils.hpp"
#include <cstdarg>
#include <iomanip>
#include <iostream>
#include "Debug.hpp"
#include "Object.hpp"
#include "Runtime.hpp"

std::string repeatString(int count, const std::string& str) {
    std::string result;
    for (int i = 0; i < count; i++) {
        result += str;
    }
    return result;
}

[[noreturn]] void panic(char const* const format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}

std::string type2String(ValueType type) {
    switch (type) {
        case Bool:
            return "bool";
        case Double:
            return "double";
        case Int:
            return "int";
        case String:
            return "string";
        case Null:
            return "null";
        case Char:
            return "char";
        case Array:
            return "array";
        case Closure:
            return "closure";
        default:
            panic("arguments with unknown type passed into %s", __func__);
    }
    return "<unknown>";
}
void checkArgsCount(int expectedCount, ObjectArray* args) {
    if (args->size() < expectedCount) {
        panic("expect %d arguments but received %d", expectedCount,
              args->size());
    }
}
void checkArgsType(int idx, ObjectArray* args, ValueType expectedType) {
    if (args->size() <= idx) {
        panic("missing arguments");
    }
    if (!args->at(idx)->isType(expectedType)) {
        panic("argument at %d has unexpected type", idx);
    }
}
void checkObjectType(const Object* object, ValueType t) {
    if (object == nullptr || object->getType() != t) {
        panic("object(%p) is expected %d but got %d", object, object->getType(),
              t);
    }
}
