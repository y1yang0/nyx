#include "Ast.h"

std::string Expression::astString() { return "Expr()"; }

std::string BoolExpr::astString() {
    return "BoolExpr(" + std::to_string(literal) + ")";
}

std::string NullExpr::astString() { return "NullExpr()"; }

std::string IntExpr::astString() {
    return "IntExpr(" + std::to_string(literal) + ")";
}

std::string DoubleExpr::astString() {
    return "DoubleExpr(" + std::to_string(literal) + ")";
}

std::string StringExpr::astString() { return "StringExpr(" + literal + ")"; }

std::string IdentExpr::astString() { return "IdentExpr(" + identName + ")"; }

std::string BinaryExpr::astString() {
    std::string str = "BinaryExpr(";
    if (opt != INVALID) {
        str += "operator=";
        str += opt;
    }
    if (lhs) {
        str += ",lhs=";
        str += lhs->astString();
    }
    if (rhs) {
        str += ",rhs=";
        str += rhs->astString();
    }
    str += ")";
    return str;
}

std::string FunCallExpr::astString() {
    std::string str = "FunCallExpr(func=";
    str += funcName;
    str += ",args=[";
    for (auto& arg : args) {
        str += arg->astString();
        str += ",";
    }
    str += "])";
    return str;
}

std::string AssignExpr::astString() {
    std::string str = "AssignExpr(varName=";
    str += identName;
    str += ",rhs=";
    str += expr->astString();
    str += ")";
    return str;
}
