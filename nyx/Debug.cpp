// MIT License
//
// Copyright (c) 2023 y1yang0 <kelthuzadx@qq.com>
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

#include "Debug.hpp"
#include <iostream>
#include "Parser.h"
#include "Runtime.hpp"
#include "Utils.hpp"

void printLex(const std::string& fileName) {
    Parser p(fileName);
    std::tuple<Token, std::string> tk;
    do {
        tk = p.next();
        std::cout << "[" << std::get<0>(tk) << "," << std::get<1>(tk) << "]\n";
    } while (std::get<0>(tk) != TK_EOF);
}

void AstDumper::visitBoolExpr(BoolExpr* node) {
    printPadding();
    std::cout << "-BoolExpr[" << node->literal << "]" << std::endl;
}
void AstDumper::visitCharExpr(CharExpr* node) {
    printPadding();
    std::cout << "-CharExpr[" << node->literal << "]" << std::endl;
}
void AstDumper::visitNullExpr(NullExpr* node) {
    printPadding();
    std::cout << "-NullExpr[null]" << std::endl;
}
void AstDumper::visitNameExpr(NameExpr* node) {
    printPadding();
    std::cout << "-NameExpr[" << node->identName << "]" << std::endl;
}
void AstDumper::visitIntExpr(IntExpr* node) {
    printPadding();
    std::cout << "-IntExpr[" << node->literal << "]" << std::endl;
}
void AstDumper::visitDoubleExpr(DoubleExpr* node) {
    printPadding();
    std::cout << "-DoubleExpr[" << node->literal << "]" << std::endl;
}
void AstDumper::visitStringExpr(StringExpr* node) {
    printPadding();
    std::cout << "-StringExpr[" << node->literal << "]" << std::endl;
}
void AstDumper::visitArrayExpr(ArrayExpr* node) {
    printPadding();
    std::cout << "-ArrayExpr" << std::endl;
    ident += 2;
    for (const auto& item : node->literal) {
        item->visit(this);
    }
    ident -= 2;
}
void AstDumper::visitIndexExpr(IndexExpr* node) {
    printPadding();
    std::cout << "-IndexExpr[" << node->identName << "]" << std::endl;
    ident += 2;
    node->index->visit(this);
    ident -= 2;
}
void AstDumper::visitBinaryExpr(BinaryExpr* node) {
    printPadding();
    std::cout << "-BinaryExpr[" << node->opt << "]" << std::endl;
    ident += 2;
    if (node->lhs != nullptr) {
        node->lhs->visit(this);
    }
    if (node->rhs != nullptr) {
        node->rhs->visit(this);
    }
    ident -= 2;
}
void AstDumper::visitFunCallExpr(FunCallExpr* node) {
    printPadding();
    std::cout << "-FunCallExpr[" << node->funcName << "]" << std::endl;
    ident += 2;
    if (node->receiver != nullptr) {
        node->receiver->visit(this);
    }
    for (const auto& item : node->args) {
        item->visit(this);
    }
    ident -= 2;
}
void AstDumper::visitAssignExpr(AssignExpr* node) {
    printPadding();
    std::cout << "-AssignExpr[" << node->opt << "]" << std::endl;
    ident += 2;
    if (node->lhs != nullptr) {
        node->lhs->visit(this);
    }
    if (node->rhs != nullptr) {
        node->rhs->visit(this);
    }
    ident -= 2;
}
void AstDumper::visitClosureExpr(ClosureExpr* node) {
    printPadding();
    std::cout << "-ClosureExpr[" << node->params.size() << "]" << std::endl;
    ident += 2;
    if (node->block != nullptr) {
        for (const auto& item : node->block->stmts) {
            item->visit(this);
        }
    }
    ident -= 2;
}
void AstDumper::visitBreakStmt(BreakStmt* node) {
    printPadding();
    std::cout << "-BreakStmt" << std::endl;
}
void AstDumper::visitContinueStmt(ContinueStmt* node) {
    printPadding();
    std::cout << "-ContinueStmt" << std::endl;
}
void AstDumper::visitSimpleStmt(SimpleStmt* node) {
    printPadding();
    std::cout << "-SimpleStmt" << std::endl;
    ident += 2;
    if (node->expr != nullptr) {
        node->expr->visit(this);
    }
    ident -= 2;
}
void AstDumper::printPadding() const {
    for (int i = 0; i < ident; i++) {
        std::cout << " ";
    }
}
void AstDumper::visitReturnStmt(ReturnStmt* node) {
    printPadding();
    std::cout << "-ReturnStmt" << std::endl;
    ident += 2;
    if (node->ret != nullptr) {
        node->ret->visit(this);
    }
    ident -= 2;
}
void AstDumper::visitIfStmt(IfStmt* node) {
    printPadding();
    std::cout << "-IfStmt" << std::endl;
    ident += 2;
    if (node->cond != nullptr) {
        node->cond->visit(this);
    }
    if (node->block != nullptr) {
        for (const auto& item : node->block->stmts) {
            item->visit(this);
        }
    }
    if (node->elseBlock != nullptr) {
        for (const auto& item : node->elseBlock->stmts) {
            item->visit(this);
        }
    }
    ident -= 2;
}
void AstDumper::visitWhileStmt(WhileStmt* node) {
    printPadding();
    std::cout << "-WhileStmt" << std::endl;
    ident += 2;
    if (node->cond != nullptr) {
        node->cond->visit(this);
    }
    if (node->block != nullptr) {
        for (const auto& item : node->block->stmts) {
            item->visit(this);
        }
    }
    ident -= 2;
}
void AstDumper::visitForStmt(ForStmt* node) {
    printPadding();
    std::cout << "-ForStmt" << std::endl;
    ident += 2;
    if (node->cond != nullptr) {
        node->cond->visit(this);
    }
    if (node->init != nullptr) {
        node->init->visit(this);
    }
    if (node->post != nullptr) {
        node->post->visit(this);
    }
    if (node->block != nullptr) {
        for (const auto& item : node->block->stmts) {
            item->visit(this);
        }
    }
    ident -= 2;
}
void AstDumper::visitForEachStmt(ForEachStmt* node) {
    printPadding();
    std::cout << "-ForEachStmt" << std::endl;
    ident += 2;
    if (node->list != nullptr) {
        node->list->visit(this);
    }
    if (node->block != nullptr) {
        for (const auto& item : node->block->stmts) {
            item->visit(this);
        }
    }
    ident -= 2;
}
void AstDumper::visitMatchStmt(MatchStmt* node) {
    printPadding();
    std::cout << "-MatchStmt" << std::endl;
    ident += 2;
    if (node->cond != nullptr) {
        node->cond->visit(this);
    }
    for (const auto& [theCase, theBranch, isAny] : node->matches) {
        theCase->visit(this);
        ident += 2;
        if (theBranch != nullptr) {
            for (const auto& item : theBranch->stmts) {
                item->visit(this);
            }
        }
        ident -= 2;
    }
    ident -= 2;
}
