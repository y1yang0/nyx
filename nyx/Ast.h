#pragma once
#include <deque>
#include <map>
#include "Nyx.hpp"

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
    TK_COMMA,      // ,
    TK_LPAREN,     // (
    TK_RPAREN,     // )
    TK_LBRACE,     // {
    TK_RBRACE,     // }
    TK_LBRACKET,   // [
    TK_RBRACKET,   // ]

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
};

using nyx::Block;
using nyx::Context;
using nyx::ExecResult;
using nyx::Runtime;
using nyx::Value;
struct Expression;
struct Statement;

struct AstNode {
    explicit AstNode(int line, int column) : line(line), column(column) {}
    virtual ~AstNode() = default;

    virtual std::string astString() { return "AstNode()"; }

    int line = -1;
    int column = -1;
};

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//
struct Expression : public AstNode {
    using AstNode::AstNode;

    virtual ~Expression() = default;

    virtual Value eval(Runtime* rt, std::deque<Context*> ctxChain);

    std::string astString() override;
};

struct BoolExpr : public Expression {
    explicit BoolExpr(int line, int column) : Expression(line, column) {}
    bool literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct CharExpr : public Expression {
    explicit CharExpr(int line, int column) : Expression(line, column) {}

    char literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct NullExpr : public Expression {
    explicit NullExpr(int line, int column) : Expression(line, column) {}

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct IntExpr : public Expression {
    explicit IntExpr(int line, int column) : Expression(line, column) {}

    int literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct DoubleExpr : public Expression {
    explicit DoubleExpr(int line, int column) : Expression(line, column) {}

    double literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct StringExpr : public Expression {
    explicit StringExpr(int line, int column) : Expression(line, column) {}

    std::string literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString();
};

struct ArrayExpr : public Expression {
    explicit ArrayExpr(int line, int column) : Expression(line, column) {}

    std::vector<Expression*> literal;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString();
};

struct IdentExpr : public Expression {
    explicit IdentExpr(std::string identName, int line, int column)
        : Expression(line, column), identName(std::move(identName)) {}
    std::string identName;
    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;

    std::string astString() override;
};

struct IndexExpr : public Expression {
    explicit IndexExpr(int line, int column) : Expression(line, column) {}

    std::string identName;
    Expression* index;

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct BinaryExpr : public Expression {
    explicit BinaryExpr(int line, int column) : Expression(line, column) {}
    Expression* lhs{};
    Token opt{};
    Expression* rhs{};
    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;

    std::string astString() override;
};

struct FunCallExpr : public Expression {
    explicit FunCallExpr(int line, int column) : Expression(line, column) {}
    std::string funcName;
    std::vector<Expression*> args;
    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct AssignExpr : public Expression {
    explicit AssignExpr(int line, int column) : Expression(line, column) {}

    Expression* lhs{};
    Token opt;
    Expression* rhs{};

    Value eval(Runtime* rt, std::deque<Context*> ctxChain) override;

    std::string astString() override;
};

//===----------------------------------------------------------------------===//
// Statement
//===----------------------------------------------------------------------===//
struct Statement : public AstNode {
    using AstNode::AstNode;

    virtual ~Statement() = default;
    virtual ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain);

    std::string astString() override;
};

struct BreakStmt : public Statement {
    explicit BreakStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct ContinueStmt : public Statement {
    explicit ContinueStmt(int line, int column) : Statement(line, column) {}

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(Expression* expr, int line, int column)
        : Statement(line, column), expr(expr) {}

    Expression* expr{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct ReturnStmt : public Statement {
    explicit ReturnStmt(int line, int column) : Statement(line, column) {}

    Expression* ret{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct IfStmt : public Statement {
    explicit IfStmt(int line, int column) : Statement(line, column) {}

    Expression* cond{};
    Block* block{};
    Block* elseBlock{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};

struct WhileStmt : public Statement {
    explicit WhileStmt(int line, int column) : Statement(line, column) {}

    Expression* cond{};
    Block* block{};

    ExecResult interpret(Runtime* rt, std::deque<Context*> ctxChain) override;
    std::string astString() override;
};
