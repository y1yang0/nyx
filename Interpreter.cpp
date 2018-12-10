#include <memory>
#include "Interpreter.h"
#include "Nyx.h"

using namespace std;

void NyxInterpreter::execute() {
    shared_ptr<GlobalContext> ctx = p.parse();

    for (int i = 0; i < ctx->stmts.size(); i++) {
        ctx->stmts[i]->interpret(ctx);
    }
}

void IfStmt::interpret(shared_ptr<GlobalContext> ctx) {}

void WhileStmt::interpret(shared_ptr<GlobalContext> ctx) {}

void ExpressionStmt::interpret(shared_ptr<GlobalContext> ctx) {}

Value BoolExpr::eval(LocalContext ctx) {
    Value val;
    val.type = nyx::ValueType::NyxBool;
    val.data = this->literal;
    return val;
}
Value IntExpr::eval(LocalContext ctx) {
    Value val;
    val.type = nyx::ValueType::NyxInt;
    val.data = this->literal;
    return val;
}
Value DoubleExpr::eval(LocalContext ctx) {
    Value val;
    val.type = nyx::ValueType::NyxDouble;
    val.data = this->literal;
    return val;
}
Value StringExpr::eval(LocalContext ctx) {
    Value val;
    val.type = nyx::ValueType::NyxString;
    val.data = this->literal;
    return val;
}
Value IdentExpr::eval(LocalContext ctx) {
    Value result;
    for (auto &v : ctx.vars) {
        if (v->name == this->identName) {
            result.type = v->value.type;
            result.data = v->value.data;
            return result;
        }
    }
    return Value(nyx::NyxNull);
}
Value BinaryExpr::eval(LocalContext ctx) {
    Value lhs = this->lhs->eval(ctx);
    Token opt = this->opt;
    Value rhs =
        this->rhs != nullptr ? this->rhs->eval(ctx) : Value(nyx::NyxNull);

    if (rhs.type != nyx::NyxNull) {
        // Binary
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
                        any_cast<int>(lhs.data) / any_cast<int>(rhs.data);
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
    } else {
        // Unary
        // Only -num supported now;
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
}
Value FunCallExpr::eval(LocalContext ctx) {}

Value AssignExpr::eval(LocalContext ctx) {
    Value lhs = this->expr->eval(ctx);

    shared_ptr<Variable> var(new Variable);

    var->name = this->identName;
    var->value = any_cast<Value>(lhs.data);
    ctx.vars.push_back(var);

    return lhs;
}
