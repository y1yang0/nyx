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
#ifndef NYX_DEBUG_HPP
#define NYX_DEBUG_HPP

#include <deque>
#include <iostream>
#include <string>
#include "Ast.h"
#include "Object.hpp"
#include "Runtime.hpp"

//===----------------------------------------------------------------------===//
// Print ast tree from root node to human-readable text
//===----------------------------------------------------------------------===//
struct AstDumper : public AstVisitor {
    int ident = 0;

    explicit AstDumper(int ident) : ident(ident) {}
    explicit AstDumper() : ident(0) {}

    void printPadding() const;
    void visitExpression(Expression* node) override {}
    void visitBoolExpr(BoolExpr* node) override;
    void visitCharExpr(CharExpr* node) override;
    void visitNullExpr(NullExpr* node) override;
    void visitIntExpr(IntExpr* node) override;
    void visitDoubleExpr(DoubleExpr* node) override;
    void visitStringExpr(StringExpr* node) override;
    void visitArrayExpr(ArrayExpr* node) override;
    void visitNameExpr(NameExpr* node) override;
    void visitIndexExpr(IndexExpr* node) override;
    void visitBinaryExpr(BinaryExpr* node) override;
    void visitFunCallExpr(FunCallExpr* node) override;
    void visitAssignExpr(AssignExpr* node) override;
    void visitClosureExpr(ClosureExpr* node) override;
    void visitStatement(Statement* node) override {}
    void visitBreakStmt(BreakStmt* node) override;
    void visitContinueStmt(ContinueStmt* node) override;
    void visitSimpleStmt(SimpleStmt* node) override;
    void visitReturnStmt(ReturnStmt* node) override;
    void visitIfStmt(IfStmt* node) override;
    void visitWhileStmt(WhileStmt* node) override;
    void visitForStmt(ForStmt* node) override;
    void visitForEachStmt(ForEachStmt* node) override;
    void visitMatchStmt(MatchStmt* node) override;
};

void printLex(const std::string& fileName);

#endif  // NYX_DEBUG_HPP
