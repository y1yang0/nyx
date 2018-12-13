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
        str += "opt=";
        switch (opt) {
            case TK_BITAND:
                str += "&";
                break;
            case TK_BITOR:
                str += "|";
                break;
            case TK_BITNOT:
                str += "!";
                break;
            case TK_LOGAND:
                str += "&&";
                break;
            case TK_LOGOR:
                str += "||";
                break;
            case TK_LOGNOT:
                str += "!";
                break;
            case TK_PLUS:
                str += "+";
                break;
            case TK_MINUS:
                str += "-";
                break;
            case TK_TIMES:
                str += "*";
                break;
            case TK_DIV:
                str += "/";
                break;
            case TK_MOD:
                str += "%";
                break;
            case TK_EQ:
                str += "==";
                break;
            case TK_NE:
                str += "!=";
                break;
            case TK_GT:
                str += ">";
                break;
            case TK_GE:
                str += ">=";
                break;
            case TK_LT:
                str += "<";
                break;
            case TK_LE:
                str += "<=";
                break;
            case TK_ASSIGN:
                str += "=";
                break;
            default:
                str += std::to_string(opt);
                break;
        }
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
    std::string str = "AssignExpr(identName=";
    str += identName;
    str += ",rhs=";
    str += expr->astString();
    str += ")";
    return str;
}
