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
#include <iostream>
#include <vector>
#include "Ast.h"
#include "Debug.hpp"
#include "Object.hpp"
#include "Runtime.hpp"
#include "Utils.hpp"

Object* nyx_builtin_print(Runtime* rt,
                          ContextChain* ctxChain,
                          ObjectArray args) {
    for (auto arg : args) {
        std::cout << arg->toString();
    }
    return rt->newObject((int)args.size());
}

Object* nyx_builtin_println(Runtime* rt,
                            ContextChain* ctxChain,
                            ObjectArray args) {
    if (!args.empty()) {
        for (auto arg : args) {
            std::cout << arg->toString() << "\n";
        }
    } else {
        std::cout << "\n";
    }

    return rt->newObject((int)args.size());
}

Object* nyx_builtin_input(Runtime* rt,
                          ContextChain* ctxChain,
                          ObjectArray args) {
    checkArgsCount(0, &args);

    std::string str;
    std::cin >> str;
    return rt->newObject(str);
}

Object* nyx_builtin_typeof(Runtime* rt,
                           ContextChain* ctxChain,
                           ObjectArray args) {
    checkArgsCount(1, &args);
    return rt->newObject(type2String(args[0]->getType()));
}

Object* nyx_builtin_length(Runtime* rt,
                           ContextChain* ctxChain,
                           ObjectArray args) {
    checkArgsCount(1, &args);

    if (args[0]->isString()) {
        return rt->newObject((int)args[0]->asString().length());
    }
    if (args[0]->isArray()) {
        return rt->newObject((int)args[0]->asArray().size());
    }

    panic(
        "TypeError: unexpected type of arguments,function %s requires string "
        "type or "
        "array type",
        __func__);
}

Object* nyx_builtin_to_int(Runtime* rt,
                           ContextChain* ctxChain,
                           ObjectArray args) {
    checkArgsCount(1, &args);
    checkArgsType(0, &args, Double);

    return rt->newObject((int)args[0]->asDouble());
}

Object* nyx_builtin_to_double(Runtime* rt,
                              ContextChain* ctxChain,
                              ObjectArray args) {
    checkArgsCount(1, &args);
    checkArgsType(0, &args, Int);

    return rt->newObject((double)args[0]->asInt());
}

Object* nyx_builtin_range(Runtime* rt,
                          ContextChain* ctxChain,
                          ObjectArray args) {
    checkArgsCount(1, &args);

    ObjectArray vals;
    if (args[0]->asInt() <= 0) {
        return rt->newObject(vals);
    }
    int start = 0, stop = 0;
    if (args.size() == 1) {
        start = 0;
        stop = args[0]->asInt();
    } else {
        start = args[0]->asInt();
        stop = args[1]->asInt();
    }
    for (; start < stop; start++) {
        vals.push_back(rt->newObject(start));
    }
    return rt->newObject(vals);
}

Object* nyx_builtin_assert(Runtime* rt,
                           ContextChain* ctxChain,
                           ObjectArray args) {
    checkArgsType(0, &args, Bool);
    if (!args[0]->asBool()) {
        if (args.size() == 2) {
            std::cerr << "AssertionFailure: " << args[1]->asString()
                      << std::endl;
        } else {
            std::cerr << "AssertionFailure" << std::endl;
        }

        std::abort();
    }
    return rt->newObject();
}

Object* nyx_builtin_dump_ast(Runtime* rt,
                             ContextChain* ctxChain,
                             ObjectArray args) {
    checkArgsCount(1, &args);
    checkArgsType(0, &args, Closure);
    auto func = args[0]->asClosure();

    std::cout << "-Func[" << func.name << "]" << std::endl;
    AstDumper d(2);
    if (func.block != nullptr) {
        for (const auto& item : func.block->stmts) {
            item->visit(&d);
        }
    }

    return nullptr;
}
