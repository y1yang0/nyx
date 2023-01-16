// MIT License
//
// Copyright (c) 2018-2023 y1yang0 <kelthuzadx@qq.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "Interpreter.h"
#include <deque>
#include <memory>
#include <vector>
#include "Ast.h"
#include "Debug.hpp"
#include "Object.hpp"
#include "Runtime.hpp"
#include "Utils.hpp"

void Interpreter::execute(Runtime* rt) {
    Interpreter::newContext(ctxChain);

    AstDumper dumper;
    for (auto stmt : rt->getStatements()) {
#if NYX_DEBUG
        stmt->visit(&dumper);
#endif
        stmt->interpret(rt, ctxChain);
    }
}

void Interpreter::newContext(ContextChain* ctxChain) {
    auto* tempContext = new Context;
    ctxChain->push_back(tempContext);
}

Object* Interpreter::callFunc(Runtime* rt,
                              Func* f,
                              ContextChain* lastCtxChain,
                              std::vector<Expression*> args) {
    ContextChain* funcCtxChain = nullptr;
    if (!f->name.empty() || f->outerContext == nullptr) {
        funcCtxChain = new ContextChain();
    } else {
        funcCtxChain = f->outerContext;
    }
    Interpreter::newContext(funcCtxChain);

    auto* funcCtx = funcCtxChain->back();
    for (int i = 0; i < f->params.size(); i++) {
        std::string paramName = f->params[i];
        // Evaluate argument values from previous context chain and push them
        // into newly created context chain
        Object* argValue = args[i]->eval(rt, lastCtxChain);
        if (argValue->isPrimitive()) {
            // Pass by value
            funcCtx->createVariable(paramName, rt->cloneObject(argValue));
        } else {
            // Pass by reference
            funcCtx->createVariable(paramName, argValue);
        }
    }

    // Execute user defined function
    ExecResult ret(ExecNormal);
    for (auto& stmt : f->block->stmts) {
        ret = stmt->interpret(rt, funcCtxChain);
        if (ret.execType == ExecReturn) {
            break;
        }
    }
    // Do not free context memory deliberately since we are not ready yet...
    return ret.retValue;
}

Object* Interpreter::evalUnaryExpr(Object* lhs, Token opt) {
    switch (opt) {
        case TK_MINUS:
            return lhs->operator-();
        case TK_LOGNOT:
            return lhs->operator!();
        case TK_BITNOT:
            return lhs->operator~();
        default:
            panic("unexpected token %d", opt);
    }

    return lhs;
}

Object* Interpreter::evalBinaryExpr(Object* lhs, Token opt, Object* rhs) {
    switch (opt) {
        case TK_PLUS:
            return lhs->operator+(rhs);
        case TK_MINUS:
            return lhs->operator-(rhs);
        case TK_TIMES:
            return lhs->operator*(rhs);
        case TK_DIV:
            return lhs->operator/(rhs);
        case TK_MOD:
            return lhs->operator%(rhs);
        case TK_LOGAND:
            return lhs->operator&&(rhs);
        case TK_LOGOR:
            return lhs->operator||(rhs);
        case TK_EQ:
            return lhs->operator==(rhs);
        case TK_NE:
            return lhs->operator!=(rhs);
            break;
        case TK_GT:
            return lhs->operator>(rhs);
        case TK_GE:
            return lhs->operator>=(rhs);
        case TK_LT:
            return lhs->operator<(rhs);
        case TK_LE:
            return lhs->operator<=(rhs);
        case TK_BITAND:
            return lhs->operator&(rhs);
        case TK_BITOR:
            return lhs->operator|(rhs);
        default:
            panic("unexpected token %d", opt);
    }
    return nullptr;
}

Object* Interpreter::assignment(Token opt, Object* lhs, Object* rhs) {
    switch (opt) {
        case TK_ASSIGN:
            return rhs;
        case TK_PLUS_AGN:
            return lhs->operator+(rhs);
        case TK_MINUS_AGN:
            return lhs->operator-(rhs);
        case TK_TIMES_AGN:
            return lhs->operator*(rhs);
        case TK_DIV_AGN:
            return lhs->operator/(rhs);
        case TK_MOD_AGN:
            return lhs->operator%(rhs);
        default:
            panic("unexpected branch reached");
    }
}

//===----------------------------------------------------------------------===//
// Interpret various statements within given runtime and context chain. Runtime
// holds all necessary data that widely used in every context. Context chain
// saves a linked contexts of current execution flow.
//===----------------------------------------------------------------------===//
ExecResult IfStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    ExecResult ret(ExecNormal);
    Object* condition = this->cond->eval(rt, ctxChain);
    if (!condition->isBool()) {
        panic(
            "expects bool type in while condition at line %d, "
            "col %d\n",
            line, column);
    }
    if (condition->asBool()) {
        Interpreter::newContext(ctxChain);
        for (auto& stmt : block->stmts) {
            ret = stmt->interpret(rt, ctxChain);
            if (ret.execType == ExecReturn) {
                break;
            } else if (ret.execType == ExecBreak) {
                break;
            } else if (ret.execType == ExecContinue) {
                break;
            }
        }

    } else {
        if (elseBlock != nullptr) {
            Interpreter::newContext(ctxChain);
            for (auto& elseStmt : elseBlock->stmts) {
                ret = elseStmt->interpret(rt, ctxChain);
                if (ret.execType == ExecReturn) {
                    break;
                } else if (ret.execType == ExecBreak) {
                    break;
                } else if (ret.execType == ExecContinue) {
                    break;
                }
            }
        }
    }
    return ret;
}

ExecResult WhileStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    ExecResult ret{ExecNormal};

    Interpreter::newContext(ctxChain);
    Object* condition = this->cond->eval(rt, ctxChain);

    while (condition->asBool()) {
        for (auto& stmt : block->stmts) {
            ret = stmt->interpret(rt, ctxChain);
            if (ret.execType == ExecReturn) {
                goto outside;
            } else if (ret.execType == ExecBreak) {
                // Disable propagating through the whole chain
                ret.execType = ExecNormal;
                goto outside;
            } else if (ret.execType == ExecContinue) {
                // Disable propagating through the whole chain
                ret.execType = ExecNormal;
                break;
            }
        }
        condition = this->cond->eval(rt, ctxChain);
        if (!condition->isBool()) {
            panic(
                "expects bool type in while condition at line %d, "
                "col %d\n",
                line, column);
        }
    }

outside:

    return ret;
}

ExecResult ForStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    ExecResult ret{ExecNormal};

    Interpreter::newContext(ctxChain);
    this->init->eval(rt, ctxChain);
    Object* condition = this->cond->eval(rt, ctxChain);

    while (condition->asBool()) {
        for (auto& stmt : block->stmts) {
            ret = stmt->interpret(rt, ctxChain);
            if (ret.execType == ExecReturn) {
                goto outside;
            } else if (ret.execType == ExecBreak) {
                ret.execType = ExecNormal;
                goto outside;
            } else if (ret.execType == ExecContinue) {
                ret.execType = ExecNormal;
                break;
            }
        }

        this->post->eval(rt, ctxChain);
        condition = this->cond->eval(rt, ctxChain);
        if (!condition->isBool()) {
            panic(
                "expects bool type in while condition at line %d, "
                "col %d\n",
                line, column);
        }
    }

outside:

    return ret;
}

ExecResult ForEachStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    ExecResult ret{ExecNormal};

    Interpreter::newContext(ctxChain);

    // Save current context for further iterator updating, we should not expect
    // to call deque.back() to get this since later statement interpretation
    // might push new context into context chain
    auto& currentCtx = ctxChain->back();
    currentCtx->createVariable(this->identName, rt->newObject());
    Object* listV = this->list->eval(rt, ctxChain);
    if (!listV->isArray()) {
        panic(
            "expects array type within foreach statement at line "
            "%d, col %d\n",
            line, column);
    }
    auto listValues = listV->asArray();
    for (auto val : listValues) {
        currentCtx->getVariable(identName)->value = val;

        for (auto stmt : this->block->stmts) {
            ret = stmt->interpret(rt, ctxChain);
            if (ret.execType == ExecReturn) {
                goto outside;
            } else if (ret.execType == ExecBreak) {
                ret.execType = ExecNormal;
                goto outside;
            } else if (ret.execType == ExecContinue) {
                ret.execType = ExecNormal;
                break;
            }
        }
    }

outside:

    return ret;
}

ExecResult MatchStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    ExecResult ret{ExecNormal};

    Object* condition =
        cond != nullptr ? this->cond->eval(rt, ctxChain) : rt->newObject(true);

    for (const auto& [theCase, theBranch, isAny] : this->matches) {
        // We must first check if it's an any(_) match because the later one
        // will actually evaluate the value of case expression, that is, the
        // identifier _ will be evaluated and might cause undefined variable
        // error.
        if (isAny || condition->equalsDeep(theCase->eval(rt, ctxChain))) {
            Interpreter::newContext(ctxChain);
            for (auto stmt : theBranch->stmts) {
                ret = stmt->interpret(rt, ctxChain);
            }

            // Stop matching and clean up context, it will propagate execution
            // type to upper statement
            goto finish;
        }
    }

finish:
    return ret;
}

ExecResult SimpleStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    this->expr->eval(rt, ctxChain);
    return ExecResult(ExecNormal);
}

ExecResult ReturnStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    if (this->ret != nullptr) {
        Object* retVal = this->ret->eval(rt, ctxChain);
        return ExecResult(ExecReturn, retVal);
    } else {
        return ExecResult(ExecReturn, nullptr);
    }
}

ExecResult BreakStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    return ExecResult(ExecBreak);
}

ExecResult ContinueStmt::interpret(Runtime* rt, ContextChain* ctxChain) {
    return ExecResult(ExecContinue);
}

//===----------------------------------------------------------------------===//
// Evaluate all expressions and return a Object structure, this object
// contains evaluated data and corresponding data type, it represents sorts
// of(also all) data type in nyx and can get value by interpreter directly.
//===----------------------------------------------------------------------===//
Object* NullExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    return rt->newObject();
}

Object* BoolExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    return rt->newObject(this->literal);
}

Object* CharExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    return rt->newObject(this->literal);
}

Object* IntExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    return rt->newObject(this->literal);
}

Object* DoubleExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    return rt->newObject(this->literal);
}

Object* StringExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    return rt->newObject(this->literal);
}

Object* ArrayExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    ObjectArray elements;
    for (auto& e : this->literal) {
        elements.push_back(e->eval(rt, ctxChain));
    }

    return rt->newObject(elements);
}

Object* ClosureExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    auto* f = new Func;
    f->params = std::move(this->params);
    f->block = this->block;
    f->outerContext = ctxChain;  // Save outer context for closure
    return rt->newObject(*f);
}

Object* NameExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    for (auto p = ctxChain->crbegin(); p != ctxChain->crend(); ++p) {
        auto* ctx = *p;
        if (auto* var = ctx->getVariable(this->identName); var != nullptr) {
            return var->value;
        }
        if (auto* var = ctx->getFunction(this->identName); var != nullptr) {
            return rt->newObject(*var);
        }
    }
    // Lookup function in global scope
    auto* globalFunc = rt->getFunction(this->identName);
    if (globalFunc != nullptr) {
        return rt->newObject(*globalFunc);
    }

    panic(
        "use of undefined variable \"%s\" at line %d, col "
        "%d\n",
        identName.c_str(), this->line, this->column);
}

Object* IndexExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    for (auto p = ctxChain->crbegin(); p != ctxChain->crend(); ++p) {
        auto* ctx = *p;
        if (auto* var = ctx->getVariable(this->identName); var != nullptr) {
            auto idx = this->index->eval(rt, ctxChain);
            if (!idx->isInt()) {
                panic(
                    "expects int type within indexing "
                    "expression at "
                    "line %d, col %d\n",
                    line, column);
            }
            if (idx->asInt() >= var->value->asArray().size()) {
                panic(
                    "index %d out of range at line %d, col "
                    "%d\n",
                    idx->asInt(), line, column);
            }
            return var->value->asArray()[idx->asInt()];
        }
    }
    panic(
        "use of undefined variable \"%s\" at line %d, col "
        "%d\n",
        identName.c_str(), this->line, this->column);
}

Object* AssignExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    Object* rhs = this->rhs->eval(rt, ctxChain);
    if (typeid(*lhs) == typeid(NameExpr)) {
        std::string identName = dynamic_cast<NameExpr*>(lhs)->identName;

        for (auto p = ctxChain->crbegin(); p != ctxChain->crend(); ++p) {
            if (auto* var = (*p)->getVariable(identName); var != nullptr) {
                var->value =
                    Interpreter::assignment(this->opt, var->value, rhs);
                return rhs;
            }
        }

        (ctxChain->back())->createVariable(identName, rhs);
    } else if (typeid(*lhs) == typeid(IndexExpr)) {
        std::string identName = dynamic_cast<IndexExpr*>(lhs)->identName;
        Object* index =
            dynamic_cast<IndexExpr*>(lhs)->index->eval(rt, ctxChain);
        if (!index->isInt()) {
            panic(
                "expects int type when applying indexing "
                "to variable %s at line %d, col %d\n",
                identName.c_str(), line, column);
        }
        for (auto p = ctxChain->crbegin(); p != ctxChain->crend(); ++p) {
            if (auto* var = (*p)->getVariable(identName); var != nullptr) {
                if (!var->value->isArray()) {
                    panic(
                        "expects array type of variable %s "
                        "at line %d, col %d\n",
                        identName.c_str(), line, column);
                }
                auto temp = var->value->asArray();
                temp[index->asInt()] = Interpreter::assignment(
                    this->opt, temp[index->asInt()], rhs);
                var->value->resetObject(temp);
                return rhs;
            }
        }

        (ctxChain->back())->createVariable(identName, rhs);
    } else {
        panic("can not assign to %s at line %d, col %d\n", typeid(lhs).name(),
              line, column);
    }
    return rhs;
}

Object* FunCallExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    if (this->receiver != nullptr) {
        // A method call, find method from receiver type
        Object* recv = receiver->eval(rt, ctxChain);
        // TODO: this dirty hack should be refactor out once we implement OOP
        // mechanism
        if (recv->isArray()) {
            if (funcName == "length") {
                return rt->newObject((int)(recv->asArray().size()));
            }
        } else if (recv->isString()) {
            if (funcName == "length") {
                return rt->newObject((int)(recv->asString().length()));
            }
        }
    }
    // Otherwise, it's a function call, find it as the builtin-in function
    // firstly then find it as user defined function, the lookup order implies
    // builtin function has higher priority when we have the same name of user
    // defined ones
    if (auto* builtinFunc = rt->getBuiltinFunction(this->funcName);
        builtinFunc != nullptr) {
        ObjectArray arguments;
        for (auto e : this->args) {
            arguments.push_back(e->eval(rt, ctxChain));
        }
        return builtinFunc(rt, ctxChain, arguments);
    }

    // Find it as a user defined function
    if (auto* normalFunc = rt->getFunction(this->funcName);
        normalFunc != nullptr) {
        if (normalFunc->params.size() != this->args.size()) {
            panic(
                "expects %d arguments but got %d at line %d, "
                "col %d\n",
                normalFunc->params.size(), this->args.size(), line, column);
        }
        return Interpreter::callFunc(rt, normalFunc, ctxChain, this->args);
    }

    // Find it as a closure function
    for (auto ctx = ctxChain->crbegin(); ctx != ctxChain->crend(); ++ctx) {
        if (auto* closure = (*ctx)->getVariable(this->funcName);
            closure != nullptr && closure->value->isClosure()) {
            auto closureFunc = closure->value->asClosure();
            if (closureFunc.params.size() != this->args.size()) {
                panic(
                    "expects %d arguments but got %d at line "
                    "%d, col %d\n",
                    closureFunc.params.size(), this->args.size(), line, column);
            }
            return Interpreter::callFunc(rt, &closureFunc, ctxChain,
                                         this->args);
        }
    }

    // Panicking since this function was not found
    panic("can not find function %s at line %d, col %d", this->funcName.c_str(),
          line, column);
}

Object* BinaryExpr::eval(Runtime* rt, ContextChain* ctxChain) {
    Object* lhsObject =
        this->lhs ? this->lhs->eval(rt, ctxChain) : rt->newObject();
    Object* rhsObject =
        this->rhs ? this->rhs->eval(rt, ctxChain) : rt->newObject();
    Token exprOpt = this->opt;

    if (!lhsObject->isNull() && rhsObject->isNull()) {
        return Interpreter::evalUnaryExpr(lhsObject, exprOpt);
    }

    return Interpreter::evalBinaryExpr(lhsObject, exprOpt, rhsObject);
}

Object* Expression::eval(Runtime* rt, ContextChain* ctxChain) {
    panic(
        "abstract expression at line %d, "
        "col "
        "%d\n",
        line, column);
}

ExecResult Statement::interpret(Runtime* rt, ContextChain* ctxChain) {
    panic(
        "abstract statement at line %d, "
        "col"
        "%d\n",
        line, column);
}
