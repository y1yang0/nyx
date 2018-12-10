#pragma once

#include <memory>
#include <string>
#include <vector>

using namespace std;

enum NyxToken {
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

struct Expression {
    virtual ~Expression() {}
};
struct Statement {
    virtual ~Statement() {}
};

struct Block {
    explicit Block() {}
    vector<shared_ptr<Statement>> stmts;
};

// Expression

struct BoolExpr : public Expression {
    explicit BoolExpr(bool literal) : literal(literal) {}
    bool literal;
};

struct IntExpr : public Expression {
    explicit IntExpr(int literal) : literal(literal) {}
    int literal;
};

struct DoubleExpr : public Expression {
    explicit DoubleExpr(double literal) : literal(literal) {}
    double literal;
};

struct StringExpr : public Expression {
    explicit StringExpr(const string& literal) : literal(literal) {}
    string literal;
};

struct IdentExpr : public Expression {
    explicit IdentExpr(const string& identName) : identName(identName) {}
    string identName;
};

struct BinaryExpr : public Expression {
    shared_ptr<Expression> lhs;
    NyxToken opt;
    shared_ptr<Expression> rhs;
};
struct UnaryExpr : public Expression {
    NyxToken opt;
    shared_ptr<Expression> expr;
};
struct FunCallExpr : public Expression {
    string funcName;
    vector<shared_ptr<Expression>> args;
};

struct AssignExpr : public Expression {
    explicit AssignExpr(const string& identName, shared_ptr<Expression> expr)
        : identName(identName), expr(expr) {}
    string identName;
    shared_ptr<Expression> expr;
};

// Statement

struct ExpressionStmt : public Statement {
    explicit ExpressionStmt(shared_ptr<Expression> expr) : expr(expr) {}
    shared_ptr<Expression> expr;
};

struct IfStmt : public Statement {
    shared_ptr<Expression> cond;
    shared_ptr<Block> block;
};

struct WhileStmt : public Statement {
    shared_ptr<Expression> cond;
    shared_ptr<Block> block;
};

// Component

struct Function {
    explicit Function() {}

    string name;
    vector<string> params;
    shared_ptr<Block> block;
};

struct Value {
    explicit Value() {}
};

struct Variable {
    explicit Variable() {}

    string name;
    shared_ptr<Value> value;
};

class Context {
    friend class Parser;

public:
    explicit Context() {}

private:
    vector<shared_ptr<Variable>> vars;
    vector<shared_ptr<Function>> funcs;
    vector<shared_ptr<Statement>> stmts;
};
