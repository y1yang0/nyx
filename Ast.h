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
struct Expression;
struct Statement;
struct Value;

// Expression
struct Expression {
    virtual ~Expression() {}
    virtual Value eval(LocalContext ctx) {}
};

struct BoolExpr : public Expression {
    explicit BoolExpr(bool literal) : literal(literal) {}
    bool literal;
    Value eval(LocalContext ctx) override;
};

struct IntExpr : public Expression {
    explicit IntExpr(int literal) : literal(literal) {}
    int literal;
    Value eval(LocalContext ctx) override;
};

struct DoubleExpr : public Expression {
    explicit DoubleExpr(double literal) : literal(literal) {}
    double literal;
    Value eval(LocalContext ctx) override;
};

struct StringExpr : public Expression {
    explicit StringExpr(const string& literal) : literal(literal) {}
    string literal;
    Value eval(LocalContext ctx) override;
};

struct IdentExpr : public Expression {
    explicit IdentExpr(const string& identName) : identName(identName) {}
    string identName;
    Value eval(LocalContext ctx) override;
};

struct BinaryExpr : public Expression {
    shared_ptr<Expression> lhs;
    Token opt;
    shared_ptr<Expression> rhs;
    Value eval(LocalContext ctx) override;
};

struct FunCallExpr : public Expression {
    string funcName;
    vector<shared_ptr<Expression>> args;
    Value eval(LocalContext ctx) override;
};

struct AssignExpr : public Expression {
    explicit AssignExpr(const string& identName, shared_ptr<Expression> expr)
        : identName(identName), expr(expr) {}
    string identName;
    shared_ptr<Expression> expr;
    Value eval(LocalContext ctx) override;
};

// Statement
struct Statement {
    virtual ~Statement() {}
    virtual void interpret(shared_ptr<GlobalContext> ctx) {}
};
struct Block {
    explicit Block() {}
    vector<shared_ptr<Statement>> stmts;
};
struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(shared_ptr<Expression> expr) : expr(expr) {}
    shared_ptr<Expression> expr;
    void interpret(shared_ptr<GlobalContext> ctx) override;
};

struct IfStmt : public Statement {
    shared_ptr<Expression> cond;
    shared_ptr<Block> block;
    void interpret(shared_ptr<GlobalContext> ctx) override;
};

struct WhileStmt : public Statement {
    shared_ptr<Expression> cond;
    shared_ptr<Block> block;
    void interpret(shared_ptr<GlobalContext> ctx) override;
};
