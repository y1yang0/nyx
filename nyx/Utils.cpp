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
            panic("TypeError: arguments with unknown type passed into %s",
                  __func__);
    }
    return "<unknown>";
}
void checkArgsCount(int expectedCount, ObjectArray* args) {
    if (args->size() < expectedCount) {
        panic("RuntimeError: expect %d arguments but received %d",
              expectedCount, args->size());
    }
}
void checkArgsType(int idx, ObjectArray* args, ValueType expectedType) {
    if (args->size() <= idx) {
        panic("RuntimeError: missing arguments");
    }
    if (!args->at(idx)->isType(expectedType)) {
        panic("TypeError: argument at %d has unexpected type", idx);
    }
}
void checkObjectType(const Object* object, ValueType t) {
    if (object == nullptr || object->getType() != t) {
        panic("TypeError: object(%p) is expected %d but got %d", object,
              object->getType(), t);
    }
}

struct DumpAst : public AstVisitor {
    int ident = 0;

    explicit DumpAst(int ident) : ident(ident) {}

    void printPadding() const {
        for (int i = 0; i < ident; i++) {
            std::cout << " ";
        }
    }

    void visitExpression(Expression* node) override {}
    void visitBoolExpr(BoolExpr* node) override {
        printPadding();
        std::cout << "-BoolExpr[" << node->literal << "]" << std::endl;
    }
    void visitCharExpr(CharExpr* node) override {
        printPadding();
        std::cout << "-CharExpr[" << node->literal << "]" << std::endl;
    }
    void visitNullExpr(NullExpr* node) override {
        printPadding();
        std::cout << "-NullExpr[null]" << std::endl;
    }
    void visitIntExpr(IntExpr* node) override {
        printPadding();
        std::cout << "-IntExpr[" << node->literal << "]" << std::endl;
    }
    void visitDoubleExpr(DoubleExpr* node) override {
        printPadding();
        std::cout << "-DoubleExpr[" << node->literal << "]" << std::endl;
    }
    void visitStringExpr(StringExpr* node) override {
        printPadding();
        std::cout << "-StringExpr[" << node->literal << "]" << std::endl;
    }
    void visitArrayExpr(ArrayExpr* node) override {
        printPadding();
        std::cout << "-ArrayExpr" << std::endl;
        ident += 2;
        for (const auto& item : node->literal) {
            item->visit(this);
        }
        ident -= 2;
    }
    void visitNameExpr(NameExpr* node) override {
        printPadding();
        std::cout << "-NameExpr[" << node->identName << "]" << std::endl;
    }
    void visitIndexExpr(IndexExpr* node) override {
        printPadding();
        std::cout << "-IndexExpr[" << node->identName << "]" << std::endl;
        ident += 2;
        node->index->visit(this);
        ident -= 2;
    }
    void visitBinaryExpr(BinaryExpr* node) override {
        printPadding();
        std::cout << "-BinaryExpr[" << node->opt << "]" << std::endl;
        ident += 2;
        node->lhs->visit(this);
        node->rhs->visit(this);
        ident -= 2;
    }
    void visitFunCallExpr(FunCallExpr* node) override {
        printPadding();
        std::cout << "-FunCallExpr[" << node->funcName << "]" << std::endl;
        ident += 2;
        for (const auto& item : node->args) {
            item->visit(this);
        }
        ident -= 2;
    }
    void visitAssignExpr(AssignExpr* node) override {
        printPadding();
        std::cout << "-AssignExpr[" << node->opt << "]" << std::endl;
        ident += 2;
        if (node->lhs != nullptr) {
            node->lhs->visit(this);
        }
        if (node->rhs != nullptr) {
            node->rhs->visit(this);
        }
        ident -= 2;
    }
    void visitClosureExpr(ClosureExpr* node) override {
        printPadding();
        std::cout << "-ClosureExpr[" << node->params.size() << "]" << std::endl;
        ident += 2;
        if (node->block != nullptr) {
            for (const auto& item : node->block->stmts) {
                item->visit(this);
            }
        }
        ident -= 2;
    }
    void visitStatement(Statement* node) override {}
    void visitBreakStmt(BreakStmt* node) override {
        printPadding();
        std::cout << "-BreakStmt" << std::endl;
    }
    void visitContinueStmt(ContinueStmt* node) override {
        printPadding();
        std::cout << "-ContinueStmt" << std::endl;
    }
    void visitSimpleStmt(SimpleStmt* node) override {}
    void visitReturnStmt(ReturnStmt* node) override {
        printPadding();
        std::cout << "-ReturnStmt" << std::endl;
        ident += 2;
        if (node->ret != nullptr) {
            node->ret->visit(this);
        }
    }
    void visitIfStmt(IfStmt* node) override {
        printPadding();
        std::cout << "-IfStmt" << std::endl;
        ident += 2;
        if (node->cond != nullptr) {
            node->cond->visit(this);
        }
        if (node->block != nullptr) {
            for (const auto& item : node->block->stmts) {
                item->visit(this);
            }
        }
        if (node->elseBlock != nullptr) {
            for (const auto& item : node->elseBlock->stmts) {
                item->visit(this);
            }
        }
        ident -= 2;
    }
    void visitWhileStmt(WhileStmt* node) override {
        printPadding();
        std::cout << "-WhileStmt" << std::endl;
        ident += 2;
        if (node->cond != nullptr) {
            node->cond->visit(this);
        }
        if (node->block != nullptr) {
            for (const auto& item : node->block->stmts) {
                item->visit(this);
            }
        }
        ident -= 2;
    }
    void visitForStmt(ForStmt* node) override {
        printPadding();
        std::cout << "-ForStmt" << std::endl;
        ident += 2;
        if (node->cond != nullptr) {
            node->cond->visit(this);
        }
        if (node->init != nullptr) {
            node->init->visit(this);
        }
        if (node->post != nullptr) {
            node->post->visit(this);
        }
        if (node->block != nullptr) {
            for (const auto& item : node->block->stmts) {
                item->visit(this);
            }
        }
        ident -= 2;
    }
    void visitForEachStmt(ForEachStmt* node) override {
        printPadding();
        std::cout << "-ForEachStmt" << std::endl;
        ident += 2;
        if (node->list != nullptr) {
            node->list->visit(this);
        }
        if (node->block != nullptr) {
            for (const auto& item : node->block->stmts) {
                item->visit(this);
            }
        }
        ident -= 2;
    }
    void visitMatchStmt(MatchStmt* node) override {
        printPadding();
        std::cout << "-MatchStmt" << std::endl;
        ident += 2;
        if (node->cond != nullptr) {
            node->cond->visit(this);
        }
        for (const auto& [theCase, theBranch, isAny] : node->matches) {
            theCase->visit(this);
            ident += 2;
            if (theBranch != nullptr) {
                for (const auto& item : theBranch->stmts) {
                    item->visit(this);
                }
            }
            ident -= 2;
        }
        ident -= 2;
    }
};

void dumpFuncAst(Func* func) {
    std::cout << "-Func[" << func->name << "]" << std::endl;
    DumpAst d(2);
    if (func->block != nullptr) {
        for (const auto& item : func->block->stmts) {
            item->visit(&d);
        }
    }
}
