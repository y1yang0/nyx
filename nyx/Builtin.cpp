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
#include <iostream>
#include <vector>
#include "Ast.h"
#include "Builtin.h"
#include "Runtime.hpp"
#include "Utils.hpp"
#include "Object.hpp"

Object *nyx_builtin_print(Runtime *rt,
                          std::deque<Context *> *ctxChain,
                          std::vector<Object *> args) {
    for (auto arg: args) {
        std::cout << arg->toString();
    }
    return rt->newObject(Int, (int) args.size());
}

Object *nyx_builtin_println(Runtime *rt,
                            std::deque<Context *> *ctxChain,
                            std::vector<Object *> args) {
    if (!args.empty()) {
        for (auto arg: args) {
            std::cout << arg->toString() << "\n";
        }
    } else {
        std::cout << "\n";
    }

    return rt->newObject(Int, (int) args.size());
}

Object *nyx_builtin_input(Runtime *rt,
                          std::deque<Context *> *ctxChain,
                          std::vector<Object *> args) {
    Object *result = rt->newObject(String, "");

    std::string str;
    std::cin >> str;
    result->set(std::make_any<std::string>(std::move(str)));
    return result;
}

Object *nyx_builtin_typeof(Runtime *rt,
                           std::deque<Context *> *ctxChain,
                           std::vector<Object *> args) {
    if (args.size() != 1) {
        panic("ArgumentError:function %s expects one argument but got %d",
              __func__, args.size());
    }
    Object *result = rt->newObject(String, "");
    result->set<std::string>(type2String(args[0]->getType()));
    return result;
}

Object *nyx_builtin_length(Runtime *rt,
                           std::deque<Context *> *ctxChain,
                           std::vector<Object *> args) {
    if (args.size() != 1) {
        panic("ArgumentError:function %s expects one argument but got %d",
              __func__, args.size());
    }

    if (args[0]->isType<String>()) {
        return rt->newObject(
                Int, std::make_any<int>(args[0]->as<std::string>().length()));
    }
    if (args[0]->isType<Array>()) {
        return rt->newObject(
                Int,
                std::make_any<int>(args[0]->as<std::vector<Object *>>().size()));
    }

    panic(
            "TypeError: unexpected type of arguments,function %s requires string "
            "type or "
            "array type",
            __func__);
}

Object *nyx_builtin_to_int(Runtime *rt,
                           std::deque<Context *> *ctxChain,
                           std::vector<Object *> args) {
    if (args.size() != 1) {
        panic("ArgumentError:function %s expects one argument but got %d",
              __func__, args.size());
    }

    if (args[0]->isType<Double>()) {
        return rt->newObject(Int, std::make_any<int>(args[0]->as<double>()));
    }
    panic("TypeError:function %s unexpected type of arguments within to_int()",
          __func__);
}

Object *nyx_builtin_to_double(Runtime *rt,
                              std::deque<Context *> *ctxChain,
                              std::vector<Object *> args) {
    if (args.size() != 1) {
        panic("ArgumentError:function %s expects one argument but got %d",
              __func__, args.size());
    }

    if (args[0]->isType<Int>()) {
        return rt->newObject(Double,
                             std::make_any<double>(args[0]->as<int>()));
    }
    panic("TypeError: unexpected type of arguments within to_double()");
}

Object *nyx_builtin_range(Runtime *rt,
                          std::deque<Context *> *ctxChain,
                          std::vector<Object *> args) {
    if (args.size() > 2) {
        panic("ArgumentError:function %s expects one or two argument but got %d",
                __func__, args.size());
    }

    if (args[0]->isType<Int>()) {
        std::vector<Object *> vals;
        if (args[0]->as<int>() <= 0) {
            return rt->newObject(Array, vals);
        }
        int start = 0, stop = 0;
        if (args.size() == 1) {
            start = 0;
            stop = args[0]->as<int>();
        } else {
            start = args[0]->as<int>();
            stop = args[1]->as<int>();
        }
        for (; start < stop; start++) {
            vals.push_back(rt->newObject(Int, start));
        }
        return rt->newObject(Array, vals);
    }
    panic("TypeError: unexpected type of arguments within %s", __func__);
}


Object *nyx_builtin_assert(Runtime *rt,
                          std::deque<Context *> *ctxChain,
                          std::vector<Object *> args) {
    if (args.size() > 2) {
        panic("ArgumentError:function %s expects one or two argument but got %d",
                __func__, args.size());
    }

    if (args[0]->isType<Bool>() && args[1]->isType<String>()) {
        if (args[0]->as<bool>() == false) {
            std::cerr<<"AssertionFailure: "<< args[1]->as<std::string>()<<std::endl;
            std::abort();
        }
        return rt->newNullObject();
    }
    panic("TypeError: unexpected type of arguments within %s", __func__);
}
