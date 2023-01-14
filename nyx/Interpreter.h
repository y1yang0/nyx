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

#include <memory>
#include "Object.hpp"
#include "Parser.h"
#include "Runtime.hpp"

//===----------------------------------------------------------------------===//
// Interpret AST nodes with execution context
//===----------------------------------------------------------------------===//
class Interpreter {
public:
    Interpreter() : ctxChain(new ContextChain) {}

    void execute(Runtime* rt);

public:
    static void newContext(ContextChain* ctxChain);

    static Object* callFunc(Runtime* rt,
                            Func* f,
                            ContextChain* lastCtxChain,
                            std::vector<Expression*> args);

    static Object* evalBinaryExpr(Object* lhs, Token opt, Object* rhs);

    static Object* evalUnaryExpr(Object* lhs, Token opt);

    static Object* assignment(Token opt, Object* lhs, Object* rhs);

private:
    ContextChain* ctxChain;
};
