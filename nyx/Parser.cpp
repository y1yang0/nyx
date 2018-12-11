#include "Parser.h"

void Parser::printLex(const string& fileName) {
    Parser p(fileName);
    std::tuple<Token, std::string> tk;
    do {
        tk = p.next();
        std::cout << "[" << std::get<0>(tk) << "," << std::get<1>(tk) << "]\n";
    } while (get<0>(tk) != TK_EOF);
}

Parser::Parser(const string& fileName) : context(new GlobalContext) {
    fs.open(fileName);
    if (!fs.is_open()) {
        cerr << "[error] can not open source file\n";
        exit(EXIT_FAILURE);
    }
}

Expression* Parser::parsePrimaryExpr() {
    if (getCurrentToken() == TK_IDENT) {
        auto ident = getCurrentLexeme();
        currentToken = next();
        if (getCurrentToken() == TK_ASSIGN) {
            currentToken = next();
            return new AssignExpr(ident, parseExpression());
        } else if (getCurrentToken() == TK_LPAREN) {
            currentToken = next();
            auto val = new FunCallExpr;
            val->funcName = ident;
            while (getCurrentToken() != TK_RPAREN) {
                val->args.push_back(parseExpression());
                if (getCurrentToken() == TK_COMMA) {
                    currentToken = next();
                }
            }
            assert(getCurrentToken() == TK_RPAREN);
            currentToken = next();
            return val;
        }
        return new IdentExpr(ident);
    } else if (getCurrentToken() == LIT_INT) {
        auto val = atoi(getCurrentLexeme().c_str());
        currentToken = next();
        return new IntExpr(val);
    } else if (getCurrentToken() == LIT_DOUBLE) {
        auto val = atof(getCurrentLexeme().c_str());
        currentToken = next();
        return new DoubleExpr(val);
    } else if (getCurrentToken() == LIT_STR) {
        auto val = getCurrentLexeme();
        currentToken = next();
        return new StringExpr(val);
    } else if (getCurrentToken() == KW_TRUE || getCurrentToken() == KW_FALSE) {
        auto val = KW_TRUE == getCurrentToken() ? true : false;
        currentToken = next();
        return new BoolExpr(val);
    } else if (getCurrentToken() == KW_NULL) {
        currentToken = next();
        return new NullExpr();
    } else if (getCurrentToken() == TK_LPAREN) {
        currentToken = next();
        auto val = parseExpression();
        assert(getCurrentToken() == TK_RPAREN);
        currentToken = next();
        return val;
    }
    return nullptr;
}

Expression* Parser::parseUnaryExpr() {
    if (getCurrentToken() == TK_MINUS) {
        auto val = new BinaryExpr;
        val->opt = getCurrentToken();
        currentToken = next();
        val->lhs = parseUnaryExpr();
        return val;
    } else if (getCurrentToken() == LIT_DOUBLE ||
               getCurrentToken() == LIT_INT || getCurrentToken() == LIT_STR ||
               getCurrentToken() == TK_IDENT ||
               getCurrentToken() == TK_LPAREN || getCurrentToken() == KW_TRUE ||
               getCurrentToken() == KW_FALSE || getCurrentToken() == KW_NULL) {
        return parsePrimaryExpr();
    }
    return nullptr;
}

BinaryExpr* Parser::parseExpression() {
    BinaryExpr* node;
    if (auto p = parseUnaryExpr(); p != nullptr) {
        node = new BinaryExpr();
        node->lhs = p;
        if (getCurrentToken() == TK_LOGOR || getCurrentToken() == TK_LOGAND ||
            getCurrentToken() == TK_EQ || getCurrentToken() == TK_NE ||
            getCurrentToken() == TK_GT || getCurrentToken() == TK_GE ||
            getCurrentToken() == TK_LT || getCurrentToken() == TK_LE ||
            getCurrentToken() == TK_PLUS || getCurrentToken() == TK_MINUS ||
            getCurrentToken() == TK_MOD || getCurrentToken() == TK_TIMES ||
            getCurrentToken() == TK_DIV) {
            node->opt = getCurrentToken();
            currentToken = next();
            node->rhs = parseExpression();
        }
    }
    return node;
}

ExpressionStmt* Parser::parseExpressionStmt() {
    ExpressionStmt* node;
    if (auto p = parseExpression(); p != nullptr) {
        node = new ExpressionStmt(p);
    }
    return node;
}

IfStmt* Parser::parseIfStmt() {
    IfStmt* node{new IfStmt};
    currentToken = next();
    node->cond = parseExpression();
    assert(getCurrentToken() == TK_RPAREN);
    currentToken = next();
    node->block = parseBlock();
    return node;
}

WhileStmt* Parser::parseWhileStmt() {
    WhileStmt* node{new WhileStmt};
    currentToken = next();
    node->cond = parseExpression();
    assert(getCurrentToken() == TK_RPAREN);
    currentToken = next();
    node->block = parseBlock();
    return node;
}

Statement* Parser::parseStatement() {
    Statement* node;
    switch (getCurrentToken()) {
        case KW_IF:
            currentToken = next();
            node = parseIfStmt();
            break;
        case KW_WHILE:
            currentToken = next();
            node = parseWhileStmt();

        default:
            node = parseExpressionStmt();
            break;
    }
    return node;
}

vector<Statement*> Parser::parseStatementList() {
    vector<Statement*> node;
    Statement* p;
    while ((p = parseStatement()) != nullptr) {
        node.push_back(p);
    }
    return node;
}

Block* Parser::parseBlock() {
    Block* node{new Block};
    currentToken = next();
    node->stmts = parseStatementList();
    assert(getCurrentToken() == TK_RBRACE);
    currentToken = next();
    return node;
}

vector<string> Parser::parseParameterList() {
    vector<string> node;
    currentToken = next();
    if (getCurrentToken() == TK_RPAREN) {
        currentToken = next();
        return move(node);
    }

    while (getCurrentToken() != TK_RPAREN) {
        if (getCurrentToken() == TK_IDENT) {
            node.push_back(getCurrentLexeme());
        } else {
            assert(getCurrentToken() == TK_COMMA);
        }
        currentToken = next();
    }
    assert(getCurrentToken() == TK_RPAREN);
    currentToken = next();
    return move(node);
}

Function* Parser::parseFuncDef() {
    assert(getCurrentToken() == KW_FUNC);
    currentToken = next();
    Function* node{new Function};
    node->name = getCurrentLexeme();
    currentToken = next();
    assert(getCurrentToken() == TK_LPAREN);
    node->params = parseParameterList();
    node->block = parseBlock();

    return node;
}

GlobalContext* Parser::parse() {
    currentToken = next();
    do {
        if (getCurrentToken() == KW_FUNC) {
            context->funcs.push_back(parseFuncDef());
        } else {
            context->stmts.push_back(parseStatement());
        }
    } while (getCurrentToken() != TK_EOF);
    return this->context;
}

tuple<Token, string> Parser::next() {
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
        while ((cn >= 'a' && cn <= 'z') || (cn >= 'A' && cn <= 'Z') ||
               (cn >= '0' && cn <= '9')) {
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
        return make_tuple(TK_TIMES, "*");
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
    cerr << "[error] unknow token fed\n";
    return make_tuple(INVALID, "invalid"s);
}
