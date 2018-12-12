#include <memory>
#include "Ast.h"
#include "Builtin.h"
#include "Interpreter.h"
#include "Nyx.h"

using namespace std;

void NyxInterpreter::execute() {
    GlobalContext* ctx = p.parse();

    for (int i = 0; i < ctx->stmts.size(); i++) {
        ctx->stmts[i]->interpret(ctx);
    }
}

void IfStmt::interpret(GlobalContext* ctx) {}

void WhileStmt::interpret(GlobalContext* ctx) {}

void ExpressionStmt::interpret(GlobalContext* ctx) {
    this->expr->eval(ctx, nullptr);
}

Value BoolExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value val;
    val.type = nyx::ValueType::NyxBool;
    val.data = this->literal;
    return val;
}
Value NullExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    return Value(nyx::NyxNull);
}
Value IntExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value val;
    val.type = nyx::ValueType::NyxInt;
    val.data = this->literal;
    return val;
}
Value DoubleExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value val;
    val.type = nyx::ValueType::NyxDouble;
    val.data = this->literal;
    return val;
}
Value StringExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value val;
    val.type = nyx::ValueType::NyxString;
    val.data = this->literal;
    return val;
}
Value IdentExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value result;
    for (auto& v : gctx->vars) {
        if (v->name == this->identName) {
            result.type = v->value.type;
            result.data = v->value.data;
            return result;
        }
    }
    return Value(nyx::NyxNull);
}

Value calcUnaryExpr(Value &lhs, Token opt) {
    Value result;
    // Only -num supported now;
    if (opt == TK_MINUS) {
        switch (lhs.type) {
        case nyx::NyxBool:
            return Value(nyx::NyxBool, !any_cast<bool>(lhs.data));
        case nyx::NyxInt:
            return Value(nyx::NyxInt, -any_cast<int>(lhs.data));
        case nyx::NyxDouble:
            return Value(nyx::NyxDouble, -any_cast<double>(lhs.data));
        default:
            throw runtime_error("invalid - operations on given value");
        }
    }

    return lhs;
}

Value calcBinaryExpr(Value &lhs, Token opt , Value& rhs) {
    Value result;
    switch (opt) {
    case TK_PLUS:
        if (lhs.type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxInt;
            result.data =
                any_cast<int>(lhs.data) + any_cast<int>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) + any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxInt &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<int>(lhs.data) + any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) + any_cast<int>(rhs.data);
        }
        break;
    case TK_MINUS:
        if (lhs.type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxInt;
            result.data =
                any_cast<int>(lhs.data) - any_cast<int>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) - any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxInt &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<int>(lhs.data) - any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) - any_cast<int>(rhs.data);
        }
        break;
    case TK_TIMES:
        if (lhs.type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxInt;
            result.data =
                any_cast<int>(lhs.data) * any_cast<int>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) * any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxInt &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<int>(lhs.data) * any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) * any_cast<int>(rhs.data);
        }
        break;
    case TK_DIV:
        if (lhs.type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxDouble;
            result.data =
                (double)any_cast<int>(lhs.data) / any_cast<int>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) / any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxInt &&
            rhs.type == nyx::NyxDouble) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<int>(lhs.data) / any_cast<double>(rhs.data);
        } else if (lhs.type == nyx::NyxDouble &&
            rhs.type == nyx::NyxInt) {
            result.type = nyx::NyxDouble;
            result.data =
                any_cast<double>(lhs.data) / any_cast<int>(rhs.data);
        }
        break;
    }
    return result;
}


Value BinaryExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value lhs = this->lhs ? this->lhs->eval(gctx, lctx) : Value(nyx::NyxNull);
    Value rhs = this->rhs ? this->rhs->eval(gctx, lctx) : Value(nyx::NyxNull);
    Token opt = this->opt;
    if (!lhs.isNyxNull()) {
        // Unary evaluating
        if (rhs.isNyxNull()) {
            return calcUnaryExpr(lhs,opt);
        }
        // Binary evaluating
        return calcBinaryExpr(lhs,opt,rhs);
    }
    return Value(nyx::NyxNull);
}
Value FunCallExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value result;
    auto func = gctx->builtin[this->funcName];
    vector<Value> arguments;
    for (auto e : this->args) {
        arguments.push_back(e->eval(gctx, lctx));
    }
    result = func(gctx, arguments);

    return result;
}

Value AssignExpr::eval(GlobalContext* gctx, LocalContext* lctx) {
    Value lhs = this->expr->eval(gctx, lctx);

    Variable* var = new Variable;

    var->name = this->identName;
    var->value = lhs;
    gctx->vars.push_back(var);

    return lhs;
}
