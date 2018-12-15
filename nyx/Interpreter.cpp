#include <deque>
#include <memory>
#include "Ast.h"
#include "Builtin.h"
#include "Interpreter.h"
#include "Nyx.hpp"
#include "Utils.hpp"

Interpreter::Interpreter(const std::string& fileName)
    : p(new Parser(fileName)) {}

Interpreter::~Interpreter() {
    delete p;
    delete nyxCtx;
}

nyx::Value Expression::eval(nyx::NyxContext* nyxCtx,
                            std::deque<nyx::Context*> ctxChain) {
    panic("RuntimeError: can not abstract expression at line %d, column %d\n",
          line, column);
}

void Interpreter::prepareContext() {
    this->nyxCtx = new nyx::NyxContext;

    nyx::Context* ctx = new nyx::Context;
    this->p->parse(nyxCtx, ctx);
    this->ctxChain.push_back(ctx);
}

void Interpreter::execute() {
    prepareContext();

    auto stmts = nyxCtx->getStatements();
    for (auto stmt : stmts) {
        stmt->interpret(nyxCtx, ctxChain);
    }
}

void Interpreter::enterContext(std::deque<nyx::Context*>& ctxChain) {
    auto* tempContext = new Context;
    ctxChain.push_back(tempContext);
}

void Interpreter::leaveContext(std::deque<nyx::Context*>& ctxChain) {
    auto* tempContext = ctxChain.back();
    ctxChain.pop_back();
    delete tempContext;
}

void IfStmt::interpret(nyx::NyxContext* nyxCtx,
                       std::deque<nyx::Context*> ctxChain) {
    Value cond = this->cond->eval(nyxCtx, ctxChain);
    if (!cond.isType<nyx::Bool>()) {
        panic(
            "RuntimeError: expect bool type in while condition at line %d, "
            "col %d\n",
            line, column);
    }
    if (true == cond.value_cast<bool>()) {
        Interpreter::enterContext(ctxChain);
        for (auto& stmt : block->stmts) {
            stmt->interpret(nyxCtx, ctxChain);
        }
        Interpreter::leaveContext(ctxChain);
    } else {
        if (elseBlock != nullptr) {
            Interpreter::enterContext(ctxChain);
            for (auto& elseStmt : elseBlock->stmts) {
                elseStmt->interpret(nyxCtx, ctxChain);
            }
            Interpreter::leaveContext(ctxChain);
        }
    }
}

void WhileStmt::interpret(nyx::NyxContext* nyxCtx,
                          std::deque<nyx::Context*> ctxChain) {
    Value cond = this->cond->eval(nyxCtx, ctxChain);

    Interpreter::enterContext(ctxChain);
    while (true == cond.value_cast<bool>()) {
        for (auto& stmt : block->stmts) {
            stmt->interpret(nyxCtx, ctxChain);
        }
        cond = this->cond->eval(nyxCtx, ctxChain);
        if (!cond.isType<nyx::Bool>()) {
            panic(
                "RuntimeError: expect bool type in while condition at line %d, "
                "col %d\n",
                line, column);
        }
    }
    Interpreter::leaveContext(ctxChain);
}

void ExpressionStmt::interpret(nyx::NyxContext* nyxCtx,
                               std::deque<nyx::Context*> ctxChain) {
    // std::cout << this->expr->astString() << "\n";
    this->expr->eval(nyxCtx, ctxChain);
}

nyx::Value NullExpr::eval(nyx::NyxContext* nyxCtx,
                          std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Null);
}
nyx::Value BoolExpr::eval(nyx::NyxContext* nyxCtx,
                          std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Bool, this->literal);
}
nyx::Value IntExpr::eval(nyx::NyxContext* nyxCtx,
                         std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Int, this->literal);
}
nyx::Value DoubleExpr::eval(nyx::NyxContext* nyxCtx,
                            std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Double, this->literal);
}
nyx::Value StringExpr::eval(nyx::NyxContext* nyxCtx,
                            std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::String, this->literal);
}
nyx::Value IdentExpr::eval(nyx::NyxContext* nyxCtx,
                           std::deque<nyx::Context*> ctxChain) {
    for (auto p = ctxChain.crbegin(); p != ctxChain.crend(); ++p) {
        auto* ctx = *p;
        if (auto* var = ctx->getVariable(this->identName); var != nullptr) {
            return var->value;
        }
    }
    panic("RuntimeError: use of undefined variable \"%s\" at line %d, col %d\n",
          identName.c_str(), this->line, this->column);
}

nyx::Value AssignExpr::eval(nyx::NyxContext* nyxCtx,
                            std::deque<nyx::Context*> ctxChain) {
    nyx::Value lhs = this->expr->eval(nyxCtx, ctxChain);

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

nyx::Value FunCallExpr::eval(nyx::NyxContext* nyxCtx,
                             std::deque<nyx::Context*> ctxChain) {
    nyx::Value result;
    if (auto* builtinFunc = nyxCtx->getBuiltinFunction(this->funcName);
        builtinFunc != nullptr) {
        std::vector<Value> arguments;
        for (auto e : this->args) {
            arguments.push_back(e->eval(nyxCtx, ctxChain));
        }
        result = builtinFunc(nyxCtx, ctxChain, arguments);
        return result;
    }
    auto* ctx = ctxChain.front();
    if (auto* f = ctx->getFunction(this->funcName); f != nullptr) {
        if (f->params.size() != this->args.size()) {
            panic("ArgumentError: expects %d arguments but got %d",
                  f->params.size(), this->args.size());
        }

        std::deque<nyx::Context*> funcCtxChain;
        Interpreter::enterContext(funcCtxChain);

        auto* funcCtx = funcCtxChain.back();
        for (int i = 0; i < f->params.size(); i++) {
            std::string paramName = f->params[i];
            nyx::Value argValue = this->args[i]->eval(nyxCtx, funcCtxChain);
            funcCtx->addVariable(f->params[i], argValue);
        }

        for (auto& stmt : f->block->stmts) {
            stmt->interpret(nyxCtx, funcCtxChain);
        }

        Interpreter::leaveContext(funcCtxChain);
        // Return null since nyx does not support return value current:(
        return nyx::Value(nyx::Null);
    }
    panic(
        "RuntimeError: can not find function definition of %s in both "
        "built-in "
        "functions and user defined functions",
        this->funcName.c_str());
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

nyx::Value BinaryExpr::eval(nyx::NyxContext* nyxCtx,
                            std::deque<nyx::Context*> ctxChain) {
    nyx::Value lhs =
        this->lhs ? this->lhs->eval(nyxCtx, ctxChain) : nyx::Value(nyx::Null);
    nyx::Value rhs =
        this->rhs ? this->rhs->eval(nyxCtx, ctxChain) : nyx::Value(nyx::Null);
    Token opt = this->opt;
    if (!lhs.isType<nyx::Null>() && rhs.isType<nyx::Null>()) {
        // Unary evaluating
        return calcUnaryExpr(lhs, opt, line, column);
    }
    // Binary evaluating
    return calcBinaryExpr(lhs, opt, rhs, line, column);
}
