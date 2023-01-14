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

#include <deque>
#include <map>
#include "Object.hpp"
#include "Runtime.hpp"

//===----------------------------------------------------------------------===//
// Token definitions of nyx
//===----------------------------------------------------------------------===//
enum Token {
    INVALID = 0,  // <invalid>
    TK_IDENT,     // <identifier>
    TK_EOF,       // <eof>
    LIT_INT,      // <integer>
    LIT_STR,      // <string>
    LIT_DOUBLE,   // <decimal>
    LIT_CHAR,     // <character>

    TK_BITAND,  // &
    TK_BITOR,   // |
    TK_BITNOT,  // ~
    TK_LOGAND,  // &&
    TK_LOGOR,   // ||
    TK_LOGNOT,  // !

    TK_PLUS,   // +
    TK_MINUS,  // -
    TK_TIMES,  // *
    TK_DIV,    // /
    TK_MOD,    // %

    TK_EQ,  // ==
    TK_NE,  // !=
    TK_GT,  // >
    TK_GE,  // >=
    TK_LT,  // <
    TK_LE,  // <=

    TK_ASSIGN,     // =
    TK_PLUS_AGN,   // +=
    TK_MINUS_AGN,  // -=
    TK_TIMES_AGN,  // *=
    TK_DIV_AGN,    // /=
    TK_MOD_AGN,    // %=
    TK_MATCH,      // =>
    TK_COMMA,      // ,
    TK_LPAREN,     // (
    TK_RPAREN,     // )
    TK_LBRACE,     // {
    TK_RBRACE,     // }
    TK_LBRACKET,   // [
    TK_RBRACKET,   // ]
    TK_SEMICOLON,  // ;
    TK_COLON,      // :

    KW_IF,        // if
    KW_ELSE,      // else
    KW_TRUE,      // true
    KW_FALSE,     // false
    KW_WHILE,     // while
    KW_FOR,       // for
    KW_NULL,      // null
    KW_FUNC,      // func
    KW_RETURN,    // return
    KW_BREAK,     // break
    KW_CONTINUE,  // continue
    KW_MATCH,     // match
};

struct Expression;
struct Statement;
struct AstNode;

struct AstVisitor;
struct BoolExpr;
struct CharExpr;
struct NullExpr;
struct IntExpr;
struct DoubleExpr;
struct StringExpr;
struct ArrayExpr;
struct NameExpr;
struct IndexExpr;
struct BinaryExpr;
struct FunCallExpr;
struct AssignExpr;
struct ClosureExpr;
struct Statement;
struct BreakStmt;
struct ContinueStmt;
struct SimpleStmt;
struct ReturnStmt;
struct IfStmt;
struct WhileStmt;
struct ForStmt;
struct ForEachStmt;
struct MatchStmt;

struct AstVisitor {
    virtual void visitExpression(Expression* node) {}
    virtual void visitBoolExpr(BoolExpr* node) {}
    virtual void visitCharExpr(CharExpr* node) {}
    virtual void visitNullExpr(NullExpr* node) {}
    virtual void visitIntExpr(IntExpr* node) {}
    virtual void visitDoubleExpr(DoubleExpr* node) {}
    virtual void visitStringExpr(StringExpr* node) {}
    virtual void visitArrayExpr(ArrayExpr* node) {}
    virtual void visitNameExpr(NameExpr* node) {}
    virtual void visitIndexExpr(IndexExpr* node) {}
    virtual void visitBinaryExpr(BinaryExpr* node) {}
    virtual void visitFunCallExpr(FunCallExpr* node) {}
    virtual void visitAssignExpr(AssignExpr* node) {}
    virtual void visitClosureExpr(ClosureExpr* node) {}
    virtual void visitStatement(Statement* node) {}
    virtual void visitBreakStmt(BreakStmt* node) {}
    virtual void visitContinueStmt(ContinueStmt* node) {}
    virtual void visitSimpleStmt(SimpleStmt* node) {}
    virtual void visitReturnStmt(ReturnStmt* node) {}
    virtual void visitIfStmt(IfStmt* node) {}
    virtual void visitWhileStmt(WhileStmt* node) {}
    virtual void visitForStmt(ForStmt* node) {}
    virtual void visitForEachStmt(ForEachStmt* node) {}
    virtual void visitMatchStmt(MatchStmt* node) {}
};

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//
struct AstNode {
    explicit AstNode(int line, int column) : line(line), column(column) {}

    virtual ~AstNode() = default;

    virtual void visit(AstVisitor* visitor) = 0;

    int line = -1;
    int column = -1;
};

struct Expression : public AstNode {
    using AstNode::AstNode;

    ~Expression() override = default;
    virtual Object* eval(Runtime* rt, ContextChain* ctxChain);

    void visit(AstVisitor* visitor) override { visitor->visitExpression(this); }
};

struct BoolExpr : public Expression {
    using Expression::Expression;

    bool literal;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;

    void visit(AstVisitor* visitor) override { visitor->visitBoolExpr(this); }
};

struct CharExpr : public Expression {
    using Expression::Expression;

    char literal;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;

    void visit(AstVisitor* visitor) override { visitor->visitCharExpr(this); }
};

struct NullExpr : public Expression {
    using Expression::Expression;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
};

struct IntExpr : public Expression {
    using Expression::Expression;

    int literal;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;

    void visit(AstVisitor* visitor) override { visitor->visitIntExpr(this); }
};

struct DoubleExpr : public Expression {
    using Expression::Expression;

    double literal;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;

    void visit(AstVisitor* visitor) override { visitor->visitDoubleExpr(this); }
};

struct StringExpr : public Expression {
    using Expression::Expression;

    std::string literal;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitStringExpr(this); }
};

struct ArrayExpr : public Expression {
    using Expression::Expression;

    std::vector<Expression*> literal;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitArrayExpr(this); }
};

struct NameExpr : public Expression {
    using Expression::Expression;

    std::string identName;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitNameExpr(this); }
};

struct IndexExpr : public Expression {
    using Expression::Expression;

    std::string identName;
    Expression* index{};

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitIndexExpr(this); }
};

struct BinaryExpr : public Expression {
    using Expression::Expression;

    Expression* lhs{};
    Token opt{};
    Expression* rhs{};

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;

    void visit(AstVisitor* visitor) override { visitor->visitBinaryExpr(this); }
};

struct FunCallExpr : public Expression {
    using Expression::Expression;

    std::string funcName;
    std::vector<Expression*> args;

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override {
        visitor->visitFunCallExpr(this);
    }
};

struct AssignExpr : public Expression {
    using Expression::Expression;

    Expression* lhs{};
    Token opt;
    Expression* rhs{};

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitAssignExpr(this); }
};

struct ClosureExpr : public Expression {
    using Expression::Expression;

    std::vector<std::string> params;
    Block* block{};

    Object* eval(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override {
        visitor->visitClosureExpr(this);
    }
};

//===----------------------------------------------------------------------===//
// Statement
//===----------------------------------------------------------------------===//
struct Statement : public AstNode {
    using AstNode::AstNode;

    virtual ~Statement() = default;

    virtual ExecResult interpret(Runtime* rt, ContextChain* ctxChain);
    void visit(AstVisitor* visitor) override { visitor->visitStatement(this); }
};

struct BreakStmt : public Statement {
    using Statement::Statement;

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitBreakStmt(this); }
};

struct ContinueStmt : public Statement {
    using Statement::Statement;

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override {
        visitor->visitContinueStmt(this);
    }
};

struct SimpleStmt : public Statement {
    using Statement::Statement;

    Expression* expr{};

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitSimpleStmt(this); }
};

struct ReturnStmt : public Statement {
    using Statement::Statement;

    Expression* ret{};

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitReturnStmt(this); }
};

struct IfStmt : public Statement {
    using Statement::Statement;

    Expression* cond{};
    Block* block{};
    Block* elseBlock{};

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitIfStmt(this); }
};

struct WhileStmt : public Statement {
    using Statement::Statement;

    Expression* cond{};
    Block* block{};

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitWhileStmt(this); }
};

struct ForStmt : public Statement {
    using Statement::Statement;

    Expression* init{};
    Expression* cond{};
    Expression* post{};
    Block* block{};

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitForStmt(this); }
};

struct ForEachStmt : public Statement {
    using Statement::Statement;

    std::string identName;
    Expression* list{};
    Block* block{};

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override {
        visitor->visitForEachStmt(this);
    }
};

struct MatchStmt : public Statement {
    using Statement::Statement;

    Expression* cond{};
    std::vector<std::tuple<Expression*, Block*, bool>> matches;

    ExecResult interpret(Runtime* rt, ContextChain* ctxChain) override;
    void visit(AstVisitor* visitor) override { visitor->visitMatchStmt(this); }
};
