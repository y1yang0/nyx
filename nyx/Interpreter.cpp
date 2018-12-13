#include <memory>
#include "Ast.h"
#include "Builtin.h"
#include "Interpreter.h"
#include "Nyx.hpp"
#include "Utils.hpp"

NyxInterpreter::NyxInterpreter(const std::string& fileName)
    : p(new Parser(fileName)) {}

NyxInterpreter::~NyxInterpreter() { delete p; }

void NyxInterpreter::execute() {
    nyx::GlobalContext* ctx = p->parse();

    for (int i = 0; i < ctx->stmts.size(); i++) {
        ctx->stmts[i]->interpret(ctx);
    }
}

void IfStmt::interpret(nyx::GlobalContext* ctx) {}

void WhileStmt::interpret(nyx::GlobalContext* ctx) {}

void ExpressionStmt::interpret(nyx::GlobalContext* ctx) {
    // std::cout << this->expr->astString() << "\n";
    this->expr->eval(ctx, nullptr);
}

nyx::Value NullExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    return nyx::Value(nyx::Null);
}
nyx::Value BoolExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    return nyx::Value(nyx::Bool, this->literal);
}
nyx::Value IntExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    return nyx::Value(nyx::Int, this->literal);
}
nyx::Value DoubleExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    return nyx::Value(nyx::Double, this->literal);
}
nyx::Value StringExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    return nyx::Value(nyx::String, this->literal);
}
nyx::Value IdentExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value result;
    for (const auto& v : gctx->vars) {
        if (v->name == this->identName) {
            result.type = v->value.type;
            result.data = v->value.data;
            return result;
        }
    }
    return nyx::Value(nyx::Null);
}

static nyx::Value calcUnaryExpr(nyx::Value& lhs, Token opt) {
    switch (opt) {
        case TK_MINUS:
            switch (lhs.type) {
                case nyx::Int:
                    return nyx::Value(nyx::Int, -std::any_cast<int>(lhs.data));
                case nyx::Double:
                    return nyx::Value(nyx::Double,
                                      -std::any_cast<double>(lhs.data));
                default:
                    throw std::runtime_error(
                        "invalid - operations on given value");
            }
            break;
        case TK_LOGNOT:
            if (lhs.type == nyx::Bool) {
                return nyx::Value(nyx::Bool, !std::any_cast<bool>(lhs.data));
            } else {
                throw std::runtime_error("invalid ! operations on given value");
            }
            break;
        case TK_BITNOT:
            if (lhs.type == nyx::Int) {
                return nyx::Value(nyx::Int, ~std::any_cast<int>(lhs.data));
            } else {
                throw std::runtime_error("invalid ~ operations on given value");
            }
            break;
    }

    // Maybe a FunCallExpr or IdentExpr
    return lhs;
}

static nyx::Value calcBinaryExpr(nyx::Value lhs, Token opt, Value rhs) {
    nyx::Value result{nyx::Null};

    switch (opt) {
        case TK_PLUS:
            result = (lhs + rhs);
            break;
        case TK_MINUS:
            result = (lhs - rhs);
            break;
        case TK_TIMES:
            result = (lhs * rhs);
            break;
        case TK_DIV:
            result = (lhs / rhs);
            break;
        case TK_MOD:
            result = (lhs % rhs);
            break;
        case TK_LOGAND:
            result = (lhs && rhs);
            break;
        case TK_LOGOR:
            result = (lhs || rhs);
            break;
        case TK_EQ:
            result = (lhs == rhs);
            break;
        case TK_NE:
            result = (lhs != rhs);
            break;
        case TK_GT:
            result = (lhs > rhs);
            break;
        case TK_GE:
            result = (lhs >= rhs);
            break;
        case TK_LT:
            result = (lhs < rhs);
            break;
        case TK_LE:
            result = (lhs <= rhs);
            break;
        case TK_BITAND:
            result = (lhs & rhs);
            break;
        case TK_BITOR:
            result = (lhs | rhs);
            break;
    }

    return result;
}

nyx::Value BinaryExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value lhs =
        this->lhs ? this->lhs->eval(gctx, lctx) : nyx::Value(nyx::Null);
    nyx::Value rhs =
        this->rhs ? this->rhs->eval(gctx, lctx) : nyx::Value(nyx::Null);
    Token opt = this->opt;
    if (!lhs.isType<nyx::Null>() && rhs.isType<nyx::Null>()) {
        // Unary evaluating
        return calcUnaryExpr(lhs, opt);
    }
    // Binary evaluating
    return calcBinaryExpr(lhs, opt, rhs);
}
nyx::Value FunCallExpr::eval(nyx::GlobalContext* gctx,
                             nyx::LocalContext* lctx) {
    nyx::Value result;
    auto func = gctx->builtin[this->funcName];
    std::vector<Value> arguments;
    for (auto e : this->args) {
        arguments.push_back(e->eval(gctx, lctx));
    }
    result = func(gctx, arguments);

    return result;
}

nyx::Value AssignExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value lhs = this->expr->eval(gctx, lctx);

    auto* var = new nyx::Variable;

    var->name = this->identName;
    var->value = lhs;
    gctx->vars.push_back(var);

    return lhs;
}
