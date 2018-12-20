#include <deque>
#include <memory>
#include <vector>
#include "Ast.h"
#include "Builtin.h"
#include "Interpreter.h"
#include "Nyx.hpp"
#include "Utils.hpp"

//===----------------------------------------------------------------------===//
// Nyx interpreter, as its name described, will interpret all statements within
// top-level source file. This part defines internal functions of interpreter
// and leaves actually statement performing later.
//===----------------------------------------------------------------------===//
Interpreter::Interpreter(const std::string& fileName)
    : p(new Parser(fileName)), rt(new nyx::Runtime) {}

Interpreter::~Interpreter() {
    delete p;
    delete rt;
}

nyx::Value Expression::eval(nyx::Runtime* rt,
                            std::deque<nyx::Context*> ctxChain) {
    panic(
        "RuntimeError: can not evaluate abstract expression at line %d, column "
        "%d\n",
        line, column);
}

nyx::ExecResult Statement::interpret(nyx::Runtime* rt,
                                     std::deque<nyx::Context*> ctxChain) {
    panic(
        "RuntimeError: can not interpret abstract statement at line %d, column "
        "%d\n",
        line, column);
}

void Interpreter::execute() {
    this->p->parse(this->rt);
    this->ctxChain.push_back(new nyx::Context);

    auto stmts = rt->getStatements();
    for (auto stmt : stmts) {
        // std::cout << stmt->astString() << "\n";
        stmt->interpret(rt, ctxChain);
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

nyx::Value Interpreter::callFunction(nyx::Runtime* rt, nyx::Function* f,
                                     std::deque<nyx::Context*> previousCtxChain,
                                     std::vector<Expression*> args) {
    // Execute user defined function
    std::deque<nyx::Context*> funcCtxChain;
    Interpreter::enterContext(funcCtxChain);

    auto* funcCtx = funcCtxChain.back();
    for (int i = 0; i < f->params.size(); i++) {
        std::string paramName = f->params[i];
        // Evaluate argument values from previouse context chain
        nyx::Value argValue = args[i]->eval(rt, previousCtxChain);
        funcCtx->createVariable(f->params[i], argValue);
    }

    nyx::ExecResult ret(nyx::ExecNormal);
    for (auto& stmt : f->block->stmts) {
        ret = stmt->interpret(rt, funcCtxChain);
        if (ret.execType == nyx::ExecReturn) {
            break;
        }
    }
    Interpreter::leaveContext(funcCtxChain);

    return ret.retValue;
}

nyx::Value Interpreter::calcUnaryExpr(nyx::Value& lhs, Token opt, int line,
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
                        "TypeError: invalid operand type for operator "
                        "-(negative) at line %d, col %d\n",
                        line, column);
            }
            break;
        case TK_LOGNOT:
            if (lhs.type == nyx::Bool) {
                return nyx::Value(nyx::Bool, !std::any_cast<bool>(lhs.data));
            } else {
                panic(
                    "TypeError: invalid operand type for operator "
                    "!(logical not) at line %d, col %d\n",
                    line, column);
            }
            break;
        case TK_BITNOT:
            if (lhs.type == nyx::Int) {
                return nyx::Value(nyx::Int, ~std::any_cast<int>(lhs.data));
            } else {
                panic(
                    "TypeError: invalid operand type for operator "
                    "~(bit not) at line %d, col %d\n",
                    line, column);
            }
            break;
    }

    return lhs;
}

nyx::Value Interpreter::calcBinaryExpr(nyx::Value lhs, Token opt, Value rhs,
                                       int line, int column) {
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

//===----------------------------------------------------------------------===//
// Interpret various statements within given runtime and context chain. Runtime
// holds all necessary data that widely used in every context. Context chain
// saves a linked contexts of current execution flow.
//===----------------------------------------------------------------------===//
nyx::ExecResult IfStmt::interpret(nyx::Runtime* rt,
                                  std::deque<nyx::Context*> ctxChain) {
    nyx::ExecResult ret(nyx::ExecNormal);
    Value cond = this->cond->eval(rt, ctxChain);
    if (!cond.isType<nyx::Bool>()) {
        panic(
            "TypeError: expects bool type in while condition at line %d, "
            "col %d\n",
            line, column);
    }
    if (true == cond.cast<bool>()) {
        Interpreter::enterContext(ctxChain);
        for (auto& stmt : block->stmts) {
            // std::cout << stmt->astString() << "\n";
            ret = stmt->interpret(rt, ctxChain);
            if (ret.execType == nyx::ExecReturn) {
                break;
            } else if (ret.execType == nyx::ExecBreak) {
                break;
            } else if (ret.execType == nyx::ExecContinue) {
                break;
            }
        }
        Interpreter::leaveContext(ctxChain);
    } else {
        if (elseBlock != nullptr) {
            Interpreter::enterContext(ctxChain);
            for (auto& elseStmt : elseBlock->stmts) {
                // std::cout << stmt->astString() << "\n";
                ret = elseStmt->interpret(rt, ctxChain);
                if (ret.execType == nyx::ExecReturn) {
                    break;
                } else if (ret.execType == nyx::ExecBreak) {
                    break;
                } else if (ret.execType == nyx::ExecContinue) {
                    break;
                }
            }
            Interpreter::leaveContext(ctxChain);
        }
    }
    return ret;
}

nyx::ExecResult WhileStmt::interpret(nyx::Runtime* rt,
                                     std::deque<nyx::Context*> ctxChain) {
    nyx::ExecResult ret;
    Value cond = this->cond->eval(rt, ctxChain);

    Interpreter::enterContext(ctxChain);
    while (true == cond.cast<bool>()) {
        for (auto& stmt : block->stmts) {
            // std::cout << stmt->astString() << "\n";
            ret = stmt->interpret(rt, ctxChain);
            if (ret.execType == nyx::ExecReturn) {
                goto outside;
            } else if (ret.execType == nyx::ExecBreak) {
                // Disable propagating through the whole chain
                ret.execType = nyx::ExecNormal;
                goto outside;
            } else if (ret.execType == nyx::ExecContinue) {
                // Disable propagating through the whole chain
                ret.execType = nyx::ExecNormal;
                break;
            }
        }
        cond = this->cond->eval(rt, ctxChain);
        if (!cond.isType<nyx::Bool>()) {
            panic(
                "TypeError: expects bool type in while condition at line %d, "
                "col %d\n",
                line, column);
        }
    }

outside:
    Interpreter::leaveContext(ctxChain);
    return ret;
}

nyx::ExecResult ExpressionStmt::interpret(nyx::Runtime* rt,
                                          std::deque<nyx::Context*> ctxChain) {
    // std::cout << this->expr->astString() << "\n";
    this->expr->eval(rt, ctxChain);
    return nyx::ExecResult(nyx::ExecNormal);
}

nyx::ExecResult ReturnStmt::interpret(nyx::Runtime* rt,
                                      std::deque<nyx::Context*> ctxChain) {
    Value retVal = this->ret->eval(rt, ctxChain);
    return nyx::ExecResult(nyx::ExecReturn, retVal);
}

nyx::ExecResult BreakStmt::interpret(nyx::Runtime* rt,
                                     std::deque<nyx::Context*> ctxChain) {
    return nyx::ExecResult(nyx::ExecBreak);
}

nyx::ExecResult ContinueStmt::interpret(nyx::Runtime* rt,
                                        std::deque<nyx::Context*> ctxChain) {
    return nyx::ExecResult(nyx::ExecContinue);
}

//===----------------------------------------------------------------------===//
// Evaulate all expressions and return a nyx::Value structure, this object
// contains evaulated data and corresponding data type, it represents sorts
// of(also all) data type in nyx and can get value by interpreter directly.
//===----------------------------------------------------------------------===//
nyx::Value NullExpr::eval(nyx::Runtime* rt,
                          std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Null);
}

nyx::Value BoolExpr::eval(nyx::Runtime* rt,
                          std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Bool, this->literal);
}

nyx::Value CharExpr::eval(nyx::Runtime* rt,
                          std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Char, this->literal);
}

nyx::Value IntExpr::eval(nyx::Runtime* rt, std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Int, this->literal);
}

nyx::Value DoubleExpr::eval(nyx::Runtime* rt,
                            std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::Double, this->literal);
}

nyx::Value StringExpr::eval(nyx::Runtime* rt,
                            std::deque<nyx::Context*> ctxChain) {
    return nyx::Value(nyx::String, this->literal);
}

nyx::Value ArrayExpr::eval(nyx::Runtime* rt,
                           std::deque<nyx::Context*> ctxChain) {
    std::vector<nyx::Value> elements;
    for (auto& e : this->literal) {
        elements.push_back(e->eval(rt, ctxChain));
    }

    return nyx::Value(nyx::Array, elements);
}

nyx::Value IdentExpr::eval(nyx::Runtime* rt,
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

nyx::Value IndexExpr::eval(nyx::Runtime* rt,
                           std::deque<nyx::Context*> ctxChain) {
    for (auto p = ctxChain.crbegin(); p != ctxChain.crend(); ++p) {
        auto* ctx = *p;
        if (auto* var = ctx->getVariable(this->identName); var != nullptr) {
            auto idx = this->index->eval(rt, ctxChain);
            if (!idx.isType<nyx::Int>()) {
                panic(
                    "TypeError: expects int type within indexing expression at "
                    "line %d, col %d\n",
                    line, column);
            }
            if (idx.cast<int>() >=
                var->value.cast<std::vector<nyx::Value>>().size()) {
                panic("IndexError: index %d out of range at line %d, col %d\n",
                      idx.cast<int>(), line, column);
            }
            return var->value.cast<std::vector<nyx::Value>>()[idx.cast<int>()];
        }
    }
    panic("RuntimeError: use of undefined variable \"%s\" at line %d, col %d\n",
          identName.c_str(), this->line, this->column);
}

nyx::Value AssignExpr::eval(nyx::Runtime* rt,
                            std::deque<nyx::Context*> ctxChain) {
    nyx::Value rhs = this->rhs->eval(rt, ctxChain);

    if (typeid(*lhs) == typeid(IdentExpr)) {
        std::string identName = dynamic_cast<IdentExpr*>(lhs)->identName;

        for (auto p = ctxChain.crbegin(); p != ctxChain.crend(); ++p) {
            if (auto* var = (*p)->getVariable(identName); var != nullptr) {
                var->value = rhs;
                return rhs;
            }
        }

        (ctxChain.back())->createVariable(identName, rhs);
    } else if (typeid(*lhs) == typeid(IndexExpr)) {
        std::string identName = dynamic_cast<IndexExpr*>(lhs)->identName;
        nyx::Value index =
            dynamic_cast<IndexExpr*>(lhs)->index->eval(rt, ctxChain);
        if (!index.isType<nyx::Int>()) {
            panic(
                "TypeError: expects int type when applying indexing "
                "to variable %s at line %d, col %d\n",
                identName.c_str(), line, column);
        }
        for (auto p = ctxChain.crbegin(); p != ctxChain.crend(); ++p) {
            if (auto* var = (*p)->getVariable(identName); var != nullptr) {
                if (!var->value.isType<nyx::Array>()) {
                    panic(
                        "TypeError: expects array type of variable %s "
                        "at line %d, col %d\n",
                        identName.c_str(), line, column);
                }
                auto&& temp = var->value.cast<std::vector<nyx::Value>>();
                temp[index.cast<int>()] = rhs;
                var->value.data = std::move(temp);
                return rhs;
            }
        }

        (ctxChain.back())->createVariable(identName, rhs);
    } else {
        panic("SyntaxError: can not assign to %s at line %d, col %d\n",
              typeid(lhs).name(), line, column);
    }
    return rhs;
}

nyx::Value FunCallExpr::eval(nyx::Runtime* rt,
                             std::deque<nyx::Context*> ctxChain) {
    if (auto* builtinFunc = rt->getBuiltinFunction(this->funcName);
        builtinFunc != nullptr) {
        std::vector<Value> arguments;
        for (auto e : this->args) {
            arguments.push_back(e->eval(rt, ctxChain));
        }
        return builtinFunc(rt, ctxChain, arguments);
    }
    if (auto* func = rt->getFunction(this->funcName); func != nullptr) {
        if (func->params.size() != this->args.size()) {
            panic("ArgumentError: expects %d arguments but got %d",
                  func->params.size(), this->args.size());
        }
        return Interpreter::callFunction(rt, func, ctxChain, this->args);
    }

    panic(
        "RuntimeError: can not find function definition of %s in both "
        "built-in "
        "functions and user defined functions",
        this->funcName.c_str());
}

nyx::Value BinaryExpr::eval(nyx::Runtime* rt,
                            std::deque<nyx::Context*> ctxChain) {
    nyx::Value lhs =
        this->lhs ? this->lhs->eval(rt, ctxChain) : nyx::Value(nyx::Null);
    nyx::Value rhs =
        this->rhs ? this->rhs->eval(rt, ctxChain) : nyx::Value(nyx::Null);
    Token opt = this->opt;

    if (!lhs.isType<nyx::Null>() && rhs.isType<nyx::Null>()) {
        return Interpreter::calcUnaryExpr(lhs, opt, line, column);
    }

    return Interpreter::calcBinaryExpr(lhs, opt, rhs, line, column);
}
