#include <utility>

#pragma once
#include <memory>
#include "Nyx.h"

enum Token {
    INVALID,        // <invalid>
    TK_COMMA,       // ,
    TK_LOGAND,      // &&
    TK_LOGOR,       // ||
    TK_LOGNOT,      // !
    TK_EQ,          // ==
    TK_ASSIGN,      // =
    TK_NE,          // !=
    TK_GT,          // >
    TK_GE,          // >=
    TK_LT,          // <
    TK_LE,          // <=
    TK_PLUS,        // +
    TK_MINUS,       // -
    TK_TIMES,       // *
    TK_DIV,         // /
    TK_MOD,         // %
    TK_LPAREN,      // (
    TK_RPAREN,      // )
    TK_LBRACE,      // {
    TK_RBRACE,      // }
    TK_LBRACKET,    // [
    TK_RBRACKET,    // ]
    TK_IDENT,       // <identifier>
    TK_EOF,         // <eof>
    KW_IF,          
    KW_TRUE,
    KW_FALSE,
    KW_WHILE,
    KW_FOR,
    KW_NULL,
    KW_FUNC,
    LIT_INT,
    LIT_STR,
    LIT_DOUBLE,
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
        return nyx::Value(nyx::NyxNull);
    }
};

struct BoolExpr : public Expression {
    explicit BoolExpr(bool literal) : literal(literal) {}
    bool literal;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};
struct NullExpr : public Expression {
    explicit NullExpr() = default;

    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};
struct IntExpr : public Expression {
    explicit IntExpr(int literal) : literal(literal) {}
    int literal;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};

struct DoubleExpr : public Expression {
    explicit DoubleExpr(double literal) : literal(literal) {}
    double literal;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};

struct StringExpr : public Expression {
    explicit StringExpr(std::string literal) : literal(std::move(literal)) {}
    std::string literal;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};

struct IdentExpr : public Expression {
    explicit IdentExpr(std::string identName) : identName(std::move(identName)) {}
    std::string identName;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};

struct BinaryExpr : public Expression {
    Expression* lhs{};
    Token opt{};
    Expression* rhs{};
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};

struct FunCallExpr : public Expression {
    std::string funcName;
    std::vector<Expression*> args;
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};

struct AssignExpr : public Expression {
    explicit AssignExpr(std::string identName, Expression* expr)
        : identName(std::move(identName)), expr(expr) {}
    std::string identName;
    Expression* expr{};
    nyx::Value eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) override;
};

// Statement
struct Statement {
    virtual ~Statement() = default;

    virtual void interpret(nyx::GlobalContext* ctx) {}
};
struct Block {
    explicit Block() = default;

    std::vector<Statement*> stmts;
};
struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(Expression* expr) : expr(expr) {}
    Expression* expr{};
    void interpret(nyx::GlobalContext* ctx) override;
};

struct IfStmt : public Statement {
    Expression* cond{};
    Block* block{};
    void interpret(nyx::GlobalContext* ctx) override;
};

struct WhileStmt : public Statement {
    Expression* cond{};
    Block* block{};
    void interpret(nyx::GlobalContext* ctx) override;
};
