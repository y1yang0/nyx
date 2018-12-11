#pragma once
#include <memory>
#include "Nyx.h"

using namespace std;

enum Token {
    INVALID,
    TK_COMMA,
    TK_LOGAND,
    TK_LOGOR,
    TK_LOGNOT,
    TK_EQ,
    TK_ASSIGN,
    TK_NE,
    TK_GT,
    TK_GE,
    TK_LT,
    TK_LE,
    TK_PLUS,
    TK_MINUS,
    TK_TIMES,
    TK_DIV,
    TK_MOD,
    TK_LPAREN,
    TK_RPAREN,
    TK_LBRACE,
    TK_RBRACE,
    TK_LBRACKET,
    TK_RBRACKET,
    TK_IDENT,
    TK_EOF,
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

using namespace std;

struct GlobalContext;
struct LocalContext;
struct Expression;
struct Statement;
struct Value;

// Expression
struct Expression {
    virtual ~Expression() {}
    virtual Value eval(GlobalContext* gctx, LocalContext* lctx) {
        return Value(nyx::NyxNull);
    }
};

struct BoolExpr : public Expression {
    explicit BoolExpr(bool literal) : literal(literal) {}
    bool literal;
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};
struct NullExpr : public Expression {
    explicit NullExpr() {}

    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};
struct IntExpr : public Expression {
    explicit IntExpr(int literal) : literal(literal) {}
    int literal;
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};

struct DoubleExpr : public Expression {
    explicit DoubleExpr(double literal) : literal(literal) {}
    double literal;
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};

struct StringExpr : public Expression {
    explicit StringExpr(const string& literal) : literal(literal) {}
    string literal;
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};

struct IdentExpr : public Expression {
    explicit IdentExpr(const string& identName) : identName(identName) {}
    string identName;
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};

struct BinaryExpr : public Expression {
    Expression* lhs{};
    Token opt{};
    Expression* rhs{};
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};

struct FunCallExpr : public Expression {
    string funcName;
    vector<Expression*> args;
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};

struct AssignExpr : public Expression {
    explicit AssignExpr(const string& identName, Expression* expr)
        : identName(identName), expr(expr) {}
    string identName;
    Expression* expr{};
    Value eval(GlobalContext* gctx, LocalContext* lctx) override;
};

// Statement
struct Statement {
    virtual ~Statement() {}
    virtual void interpret(GlobalContext* ctx) {}
};
struct Block {
    explicit Block() {}
    vector<Statement*> stmts;
};
struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(Expression* expr) : expr(expr) {}
    Expression* expr{};
    void interpret(GlobalContext* ctx) override;
};

struct IfStmt : public Statement {
    Expression* cond{};
    Block* block{};
    void interpret(GlobalContext* ctx) override;
};

struct WhileStmt : public Statement {
    Expression* cond{};
    Block* block{};
    void interpret(GlobalContext* ctx) override;
};
