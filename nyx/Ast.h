#pragma once
#include "Nyx.hpp"

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

// Expression
struct Expression {
    virtual ~Expression() = default;

    virtual nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
        return nyx::Value(nyx::Null);
    }
    virtual std::string astString();
};

struct BoolExpr : public Expression {
    explicit BoolExpr(bool literal) : literal(literal) {}
    bool literal;

    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString() override;
};
struct NullExpr : public Expression {
    explicit NullExpr() = default;

    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString() override;
};
struct IntExpr : public Expression {
    explicit IntExpr(int literal) : literal(literal) {}
    int literal;

    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString() override;
};

struct DoubleExpr : public Expression {
    explicit DoubleExpr(double literal) : literal(literal) {}
    double literal;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString() override;
};

struct StringExpr : public Expression {
    explicit StringExpr(std::string literal) : literal(std::move(literal)) {}
    std::string literal;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString();
};

struct IdentExpr : public Expression {
    explicit IdentExpr(std::string identName)
        : identName(std::move(identName)) {}
    std::string identName;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString() override;
};

struct BinaryExpr : public Expression {
    Expression* lhs{};
    Token opt{};
    Expression* rhs{};
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString() override;
};

struct FunCallExpr : public Expression {
    std::string funcName;
    std::vector<Expression*> args;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
    std::string astString() override;
};

struct AssignExpr : public Expression {
    explicit AssignExpr(std::string identName, Expression* expr)
        : identName(std::move(identName)), expr(expr) {}
    std::string identName;
    Expression* expr{};
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;

    std::string astString() override;
};

// Statement
struct Statement {
    virtual ~Statement() = default;

    virtual void interpret(nyx::GlobalContext* ctx) {}

    virtual std::string astString() { return "Stmt"; }
};
struct Block {
    explicit Block() = default;

    std::vector<Statement*> stmts;
};
struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(Expression* expr) : expr(expr) {}
    Expression* expr{};
    void interpret(nyx::GlobalContext* ctx) override;

    std::string astString() override;
};

struct IfStmt : public Statement {
    Expression* cond{};
    Block* block{};
    Block* elseBlock{};
    void interpret(nyx::GlobalContext* ctx) override;

    std::string astString() override;
};

struct WhileStmt : public Statement {
    Expression* cond{};
    Block* block{};

    void interpret(nyx::GlobalContext* ctx) override;
    std::string astString() override;
};
