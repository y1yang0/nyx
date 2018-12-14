#include <deque>
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
    nyx::GlobalContext* gctx = p->parse();

    // The first context must be GlobalContext, when we met first
    // element in the deque, we can safely convert to GlobalContex
    std::deque<nyx::LocalContext*> contextChain;
    contextChain.push_back(gctx);

    for (int i = 0; i < gctx->stmts.size(); i++) {
        gctx->stmts[i]->interpret(contextChain);
    }
    delete gctx;
}

void NyxInterpreter::enterContext(std::deque<nyx::LocalContext*>& ctxChain) {
    auto* tempContext = new LocalContext;
    ctxChain.push_back(tempContext);
}

void NyxInterpreter::leaveContext(std::deque<nyx::LocalContext*>& ctxChain) {
    auto* tempContext = ctxChain.back();
    ctxChain.pop_back();
    delete tempContext;
}

void IfStmt::interpret(std::deque<nyx::LocalContext*> ctxChain) {
    Value cond = this->cond->eval(ctxChain);
    if (!cond.isType<nyx::Bool>()) {
        panic(
            "RuntimeError: expect bool type in while condition at line %d, "
            "col %d\n",
            line, column);
    }
    if (true == cond.value_cast<bool>()) {
        NyxInterpreter::enterContext(ctxChain);
        for (auto& stmt : block->stmts) {
            stmt->interpret(ctxChain);
        }
        NyxInterpreter::leaveContext(ctxChain);
    } else {
        if (elseBlock != nullptr) {
            NyxInterpreter::enterContext(ctxChain);
            for (auto& elseStmt : elseBlock->stmts) {
                elseStmt->interpret(ctxChain);
            }
            NyxInterpreter::leaveContext(ctxChain);
        }
    }
}

void WhileStmt::interpret(std::deque<nyx::LocalContext*> ctxChain) {
    Value cond = this->cond->eval(ctxChain);

    NyxInterpreter::enterContext(ctxChain);
    while (true == cond.value_cast<bool>()) {
        for (auto& stmt : block->stmts) {
            stmt->interpret(ctxChain);
        }
        cond = this->cond->eval(ctxChain);
        if (!cond.isType<nyx::Bool>()) {
            panic(
                "RuntimeError: expect bool type in while condition at line %d, "
                "col %d\n",
                line, column);
        }
    }
    NyxInterpreter::leaveContext(ctxChain);
}

void ExpressionStmt::interpret(std::deque<nyx::LocalContext*> ctxChain) {
    // std::cout << this->expr->astString() << "\n";
    this->expr->eval(ctxChain);
}

nyx::Value NullExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    return nyx::Value(nyx::Null);
}
nyx::Value BoolExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    return nyx::Value(nyx::Bool, this->literal);
}
nyx::Value IntExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    return nyx::Value(nyx::Int, this->literal);
}
nyx::Value DoubleExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    return nyx::Value(nyx::Double, this->literal);
}
nyx::Value StringExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    return nyx::Value(nyx::String, this->literal);
}
nyx::Value IdentExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    for (auto p = ctxChain.crbegin(); p != ctxChain.crend(); ++p) {
        auto* ctx = *p;
        if (auto* var = ctx->findVariable(this->identName); var != nullptr) {
            return var->value;
        }
    }
    panic("RuntimeError: use of undefined variable \"%s\" at line %d, col %d\n",
          identName.c_str(), this->line, this->column);
}

nyx::Value AssignExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    nyx::Value lhs = this->expr->eval(ctxChain);

    // If local context exists, allocate variable within it
    // Otherwise, allocate variable in global context
    if (ctxChain.size() == 1) {
        assert(typeid(*ctxChain.back()) == typeid(GlobalContext));
    }

    // If this variable was already defined in current context or prior contexts
    // then reassign value to it
    for (auto p = ctxChain.crbegin(); p != ctxChain.crend(); ++p) {
        if ((*p)->hasVariable(identName)) {
            (*p)->removeVariable(identName);
            (*p)->addVariable(identName, lhs);
            return lhs;
        }
    }
    // Otherwise, allocate variable within current context
    (ctxChain.back())->addVariable(identName, lhs);

    return lhs;
}

nyx::Value FunCallExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    nyx::Value result;
    GlobalContext* gctx = (GlobalContext*)ctxChain.front();
    auto func = gctx->builtin[this->funcName];
    std::vector<Value> arguments;
    for (auto e : this->args) {
        arguments.push_back(e->eval(ctxChain));
    }
    result = func(gctx, arguments);

    return result;
}

static nyx::Value calcUnaryExpr(nyx::Value& lhs, Token opt, int line,
                                int column) {
    switch (opt) {
        case TK_MINUS:
            switch (lhs.type) {
                case nyx::Int:
                    return nyx::Value(nyx::Int, -std::any_cast<int>(lhs.data));
                case nyx::Double:
                    return nyx::Value(nyx::Double,
                                      -std::any_cast<double>(lhs.data));
                default:
                    panic(
                        "RuntimeError: invalid operand type for operator "
                        "-(negative) at line %d, col %d\n",
                        line, column);
            }
            break;
        case TK_LOGNOT:
            if (lhs.type == nyx::Bool) {
                return nyx::Value(nyx::Bool, !std::any_cast<bool>(lhs.data));
            } else {
                panic(
                    "RuntimeError: invalid operand type for operator "
                    "!(logical not) at line %d, col %d\n",
                    line, column);
            }
            break;
        case TK_BITNOT:
            if (lhs.type == nyx::Int) {
                return nyx::Value(nyx::Int, ~std::any_cast<int>(lhs.data));
            } else {
                panic(
                    "RuntimeError: invalid operand type for operator "
                    "~(bit not) at line %d, col %d\n",
                    line, column);
            }
            break;
    }

    // Maybe a FunCallExpr or IdentExpr
    return lhs;
}

static nyx::Value calcBinaryExpr(nyx::Value lhs, Token opt, Value rhs, int line,
                                 int column) {
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

nyx::Value BinaryExpr::eval(std::deque<nyx::LocalContext*> ctxChain) {
    nyx::Value lhs =
        this->lhs ? this->lhs->eval(ctxChain) : nyx::Value(nyx::Null);
    nyx::Value rhs =
        this->rhs ? this->rhs->eval(ctxChain) : nyx::Value(nyx::Null);
    Token opt = this->opt;
    if (!lhs.isType<nyx::Null>() && rhs.isType<nyx::Null>()) {
        // Unary evaluating
        return calcUnaryExpr(lhs, opt, line, column);
    }
    // Binary evaluating
    return calcBinaryExpr(lhs, opt, rhs, line, column);
}
