#pragma once
#include <deque>
#include <map>
#include "Nyx.hpp"

//===----------------------------------------------------------------------===//
// Token definitions of nyx
//===----------------------------------------------------------------------===//
enum Token {
    INVALID = 0,  // <invalid>

    TK_IDENT,    // <identifier>
    TK_EOF,      // <eof>
    LIT_INT,     // <integer>
    LIT_STR,     // <string>
    LIT_DOUBLE,  // <decimal>

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

    TK_ASSIGN,    // =
    TK_COMMA,     // ,
    TK_LPAREN,    // (
    TK_RPAREN,    // )
    TK_LBRACE,    // {
    TK_RBRACE,    // }
    TK_LBRACKET,  // [
    TK_RBRACKET,  // ]

    KW_IF,     // if
    KW_ELSE,   // else
    KW_TRUE,   // true
    KW_FALSE,  // false
    KW_WHILE,  // while
    KW_FOR,    // for
    KW_NULL,   // null
    KW_FUNC,   // func
};

using nyx::GlobalContext;
using nyx::LocalContext;
using nyx::Value;
struct Expression;
struct Statement;

struct AstNode {
    explicit AstNode(int line, int column) : line(line), column(column) {}
    virtual ~AstNode() = default;

    virtual std::string astString() { return "AstNode()"; }

    int line = -1;
    int column = 1;
};

//===----------------------------------------------------------------------===//
// Expression
//===----------------------------------------------------------------------===//
struct Expression : public AstNode {
    using AstNode::AstNode;

    virtual ~Expression() = default;

    virtual nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain);

    std::string astString() override;
};

struct BoolExpr : public Expression {
    explicit BoolExpr(bool literal, int line, int column)
        : Expression(line, column), literal(literal) {}
    bool literal;

    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};
struct NullExpr : public Expression {
    explicit NullExpr(int line, int column) : Expression(line, column) {}

    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};
struct IntExpr : public Expression {
    explicit IntExpr(int literal, int line, int column)
        : Expression(line, column), literal(literal) {}
    int literal;

    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};

struct DoubleExpr : public Expression {
    explicit DoubleExpr(double literal, int line, int column)
        : Expression(line, column), literal(literal) {}
    double literal;
    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};

struct StringExpr : public Expression {
    explicit StringExpr(std::string literal, int line, int column)
        : Expression(line, column), literal(std::move(literal)) {}
    std::string literal;
    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString();
};

struct IdentExpr : public Expression {
    explicit IdentExpr(std::string identName, int line, int column)
        : Expression(line, column), identName(std::move(identName)) {}
    std::string identName;
    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};

struct BinaryExpr : public Expression {
    explicit BinaryExpr(int line, int column) : Expression(line, column) {}
    Expression* lhs{};
    Token opt{};
    Expression* rhs{};
    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};

struct FunCallExpr : public Expression {
    explicit FunCallExpr(int line, int column) : Expression(line, column) {}
    std::string funcName;
    std::vector<Expression*> args;
    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;
    std::string astString() override;
};

struct AssignExpr : public Expression {
    explicit AssignExpr(std::string identName, Expression* expr, int line,
                        int column)
        : Expression(line, column),
          identName(std::move(identName)),
          expr(expr) {}
    std::string identName;
    Expression* expr{};
    nyx::Value eval(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};

//===----------------------------------------------------------------------===//
// Statement
//===----------------------------------------------------------------------===//
struct Statement : public AstNode {
    using AstNode::AstNode;

    virtual ~Statement() = default;

    virtual void interpret(std::deque<nyx::LocalContext*> ctxChain) {}

    std::string astString() override;
};
struct Block {
    explicit Block() = default;

    std::vector<Statement*> stmts;
};
struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(Expression* expr, int line, int column)
        : Statement(line, column), expr(expr) {}

    Expression* expr{};
    void interpret(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};

struct IfStmt : public Statement {
    explicit IfStmt(int line, int column) : Statement(line, column) {}

    Expression* cond{};
    Block* block{};
    Block* elseBlock{};
    void interpret(std::deque<nyx::LocalContext*> ctxChain) override;

    std::string astString() override;
};

struct WhileStmt : public Statement {
    explicit WhileStmt(int line, int column) : Statement(line, column) {}
    Expression* cond{};
    Block* block{};

    void interpret(std::deque<nyx::LocalContext*> ctxChain) override;
    std::string astString() override;
};
