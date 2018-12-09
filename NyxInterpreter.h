#pragma once

#include <memory>
#include <string>
#include <vector>

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

// Expression
struct Expression {
    virtual ~Expression() = 0;
};

struct IntExpr : public Expression {
    int literal;
};

struct DoubleExpr : public Expression {
    double literal;
};

struct StringExpr : public Expression {
    string literal;
};

struct IdentExpr : public Expression {
    string identName;
};

struct BinaryExpr : public Expression {
    shared_ptr<Expression> lhs;
    shared_ptr<Expression> rhs;
};
struct UnaryExpr : public Expression {
    shared_ptr<Expression> expr;
};
struct FunCallExpr : public Expression {};

struct AssignExpr : public Expression {};

// Statement
struct Statement {
    virtual ~Statement() = 0;
};
struct ExpressionStmt : public Statement {
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

struct Block {
    explicit Block() {}
    vector<shared_ptr<Statement>> stmts;
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

class NyxInterpreter {
    friend class Parser;

public:
    explicit NyxInterpreter() {}

private:
    vector<shared_ptr<Variable>> vars;
    vector<shared_ptr<Function>> funcs;
    vector<shared_ptr<Statement>> stmts;
};
