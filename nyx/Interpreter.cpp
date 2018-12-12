#include <memory>
#include "Ast.h"
#include "Builtin.h"
#include "Interpreter.h"
#include "Nyx.h"
#include "Utils.h"

NyxInterpreter::NyxInterpreter(const std::string& fileName) {
    p = new Parser(fileName);
}

NyxInterpreter::~NyxInterpreter() {
    delete p;
}

void NyxInterpreter::execute() {
    nyx::GlobalContext* ctx = p->parse();

    for (int i = 0; i < ctx->stmts.size(); i++) {
        ctx->stmts[i]->interpret(ctx);
    }
}

void IfStmt::interpret(nyx::GlobalContext* ctx) {}

void WhileStmt::interpret(nyx::GlobalContext* ctx) {}

void ExpressionStmt::interpret(nyx::GlobalContext* ctx) {
    //std::cout << this->expr->astString() << "\n";
    this->expr->eval(ctx, nullptr);
}

nyx::Value BoolExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value val;
    val.type = nyx::ValueType::NyxBool;
    val.data = this->literal;
    return val;
}

nyx::Value NullExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    return nyx::Value(nyx::NyxNull);
}
nyx::Value IntExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value val;
    val.type = nyx::ValueType::NyxInt;
    val.data = this->literal;
    return val;
}
nyx::Value DoubleExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value val;
    val.type = nyx::ValueType::NyxDouble;
    val.data = this->literal;
    return val;
}
nyx::Value StringExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value val;
    val.type = nyx::ValueType::NyxString;
    val.data = this->literal;
    return val;
}
nyx::Value IdentExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value result;
    for (auto& v : gctx->vars) {
        if (v->name == this->identName) {
            result.type = v->value.type;
            result.data = v->value.data;
            return result;
        }
    }
    return nyx::Value(nyx::NyxNull);
}

static nyx::Value calcUnaryExpr(nyx::Value& lhs, Token opt) {
    nyx::Value result;
    // Only -num supported now;
    if (opt == TK_MINUS) {
        switch (lhs.type) {
            case nyx::NyxBool:
                return nyx::Value(nyx::NyxBool, !std::any_cast<bool>(lhs.data));
            case nyx::NyxInt:
                return nyx::Value(nyx::NyxInt, -std::any_cast<int>(lhs.data));
            case nyx::NyxDouble:
                return nyx::Value(nyx::NyxDouble,
                                  -std::any_cast<double>(lhs.data));
            default:
                throw std::runtime_error("invalid - operations on given value");
        }
    }

    return lhs;
}

static nyx::Value calcBinaryExpr(nyx::Value lhs, Token opt, Value rhs) {
    nyx::Value result;
    switch (opt) {
        case TK_PLUS:
            result = lhs + rhs;
            break;
        case TK_MINUS:
            result = lhs - rhs;
            break;
        case TK_TIMES:
            result = lhs * rhs;
            break;
        case TK_DIV:
            result = lhs / rhs;
            break;
        case TK_MOD:
            result = lhs % rhs;
            break;
    }
    return result;
}

nyx::Value BinaryExpr::eval(nyx::GlobalContext* gctx, nyx::LocalContext* lctx) {
    nyx::Value lhs =
        this->lhs ? this->lhs->eval(gctx, lctx) : nyx::Value(nyx::NyxNull);
    nyx::Value rhs =
        this->rhs ? this->rhs->eval(gctx, lctx) : nyx::Value(nyx::NyxNull);
    Token opt = this->opt;
    if (!lhs.isNyxNull()) {
        // Unary evaluating
        if (rhs.isNyxNull()) {
            return calcUnaryExpr(lhs, opt);
        }
        // Binary evaluating
        return calcBinaryExpr(lhs, opt, rhs);
    }
    return nyx::Value(nyx::NyxNull);
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
