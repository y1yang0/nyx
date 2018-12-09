#include "Parser.h"

tuple<NyxToken, string> Parser::next() {
    char c = fs.get();

    if (c == EOF) {
        return make_tuple(TK_EOF, ""s);
    }
    if (c == ' ' || c == '\n' || c == '\r') {
        while (c == ' ' || c == '\n' || c == '\r') {
            c = fs.get();
        }
    }

    if (c == '#') {
        while (c != '\n') {
            c = fs.get();
        }
        fs.get();
        c = fs.get();
        if (c == EOF) {
            return make_tuple(TK_EOF, ""s);
        }
    }

    if (c >= '0' && c <= '9') {
        string lexeme{c};
        bool isDouble = false;
        char cn = fs.peek();
        while (cn >= '0' && cn <= '9' || (!isDouble && cn == '.')) {
            if (c == '.') {
                isDouble = true;
            }
            c = fs.get();
            cn = fs.peek();
            lexeme += c;
        }
        return !isDouble ? make_tuple(LIT_INT, lexeme)
                         : make_tuple(LIT_DOUBLE, lexeme);
    }
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        string lexeme{c};
        char cn = fs.peek();
        while ((cn >= 'a' && cn <= 'z') || (cn >= 'A' && cn <= 'Z')) {
            c = fs.get();
            lexeme += c;
            cn = fs.peek();
        }
        auto result = keywords.find(lexeme);
        return result != keywords.end() ? make_tuple(result->second, lexeme)
                                        : make_tuple(TK_IDENT, lexeme);
    }
    if (c == '"') {
        string lexeme;
        char cn = fs.peek();
        while (cn != '"') {
            c = fs.get();
            lexeme += c;
            cn = fs.peek();
        }
        fs.get();
        return make_tuple(LIT_STR, lexeme);
    }
    if (c == '[') {
        return make_tuple(TK_LBRACKET, "[");
    }
    if (c == ']') {
        return make_tuple(TK_RBRACKET, "]");
    }
    if (c == '{') {
        return make_tuple(TK_LBRACE, "{");
    }
    if (c == '}') {
        return make_tuple(TK_RBRACE, "}");
    }
    if (c == '(') {
        return make_tuple(TK_LPAREN, "(");
    }
    if (c == ')') {
        return make_tuple(TK_RPAREN, ")");
    }
    if (c == ',') {
        return make_tuple(TK_COMMA, ",");
    }
    if (c == '+') {
        return make_tuple(TK_PLUS, "+");
    }
    if (c == '-') {
        return make_tuple(TK_MINUS, "-");
    }
    if (c == '*') {
        return make_tuple(TK_PLUS, "*");
    }
    if (c == '/') {
        return make_tuple(TK_DIV, "/");
    }
    if (c == '%') {
        return make_tuple(TK_MOD, "%");
    }
    if (c == '=') {
        if (fs.peek() == '=') {
            c = fs.get();
            return make_tuple(TK_EQ, "==");
        }
        return make_tuple(TK_ASSIGN, "=");
    }
    if (c == '!') {
        if (fs.peek() == '=') {
            c = fs.get();
            return make_tuple(TK_NE, "!=");
        }
        return make_tuple(TK_LOGNOT, "!");
    }
    if (c == '|') {
        c = fs.get();
        return make_tuple(TK_LOGOR, "||");
    }
    if (c == '&') {
        c = fs.get();
        return make_tuple(TK_LOGAND, "&&");
    }
    if (c == '>') {
        if (fs.peek() == '=') {
            c = fs.get();
            return make_tuple(TK_GE, ">=");
        }
        return make_tuple(TK_GT, ">");
    }
    if (c == '<') {
        if (fs.peek() == '=') {
            c = fs.get();
            return make_tuple(TK_LE, "<=");
        }
        return make_tuple(TK_LT, "<");
    }
    cout << "[error] unknow token fed\n";
    return make_tuple(INVALID, "invalid"s);
}

tuple<NyxToken, string> Parser::expect(NyxToken tk) {
    auto res = next();
    if (get<0>(res) != tk) {
        throw runtime_error("unexpected token " + get<1>(res));
    }
    return res;
}
