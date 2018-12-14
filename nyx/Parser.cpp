#include "Nyx.hpp"
#include "Parser.h"
#include "Utils.hpp"

void Parser::printLex(const std::string& fileName) {
    Parser p(fileName);
    std::tuple<Token, std::string> tk;
    do {
        tk = p.next();
        std::cout << "[" << std::get<0>(tk) << "," << std::get<1>(tk) << "]\n";
    } while (std::get<0>(tk) != TK_EOF);
}

Parser::Parser(const std::string& fileName)
    : context(new nyx::GlobalContext),
      keywords({{"if", KW_IF},
                {"else", KW_ELSE},
                {"while", KW_WHILE},
                {"null", KW_NULL},
                {"true", KW_TRUE},
                {"false", KW_FALSE},
                {"for", KW_FALSE},
                {"func", KW_FUNC}}) {
    fs.open(fileName);
    if (!fs.is_open()) {
        panic("ParserError: can not open source file");
    }
}

Parser::~Parser() {
    // It will be released by destruction behavior of local std::deque
    // delete context;
    fs.close();
}

Expression* Parser::parsePrimaryExpr() {
    if (getCurrentToken() == TK_IDENT) {
        auto ident = getCurrentLexeme();
        currentToken = next();
        if (getCurrentToken() == TK_ASSIGN) {
            currentToken = next();
            return new AssignExpr(ident, parseExpression(), line, column);
        } else if (getCurrentToken() == TK_LPAREN) {
            currentToken = next();
            auto val = new FunCallExpr(line, column);
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
        return new IdentExpr(ident, line, column);
    } else if (getCurrentToken() == LIT_INT) {
        auto val = atoi(getCurrentLexeme().c_str());
        currentToken = next();
        return new IntExpr(val, line, column);
    } else if (getCurrentToken() == LIT_DOUBLE) {
        auto val = atof(getCurrentLexeme().c_str());
        currentToken = next();
        return new DoubleExpr(val, line, column);
    } else if (getCurrentToken() == LIT_STR) {
        auto val = getCurrentLexeme();
        currentToken = next();
        return new StringExpr(val, line, column);
    } else if (getCurrentToken() == KW_TRUE || getCurrentToken() == KW_FALSE) {
        auto val = KW_TRUE == getCurrentToken();
        currentToken = next();
        return new BoolExpr(val, line, column);
    } else if (getCurrentToken() == KW_NULL) {
        currentToken = next();
        return new NullExpr(line, column);
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
    if (anyone(getCurrentToken(), TK_MINUS, TK_LOGNOT, TK_BITNOT)) {
        auto val = new BinaryExpr(line, column);
        val->opt = getCurrentToken();
        currentToken = next();
        val->lhs = parseUnaryExpr();
        return val;
    } else if (anyone(getCurrentToken(), LIT_DOUBLE, LIT_INT, LIT_STR, TK_IDENT,
                      TK_LPAREN, KW_TRUE, KW_FALSE, KW_NULL)) {
        return parsePrimaryExpr();
    }
    return nullptr;
}

Expression* Parser::parseExpression(short oldPrecedence) {
    auto p = parseUnaryExpr();

    while (anyone(getCurrentToken(), TK_BITOR, TK_BITAND, TK_BITNOT, TK_LOGOR,
                  TK_LOGAND, TK_LOGNOT, TK_EQ, TK_NE, TK_GT, TK_GE, TK_LT,
                  TK_LE, TK_PLUS, TK_MINUS, TK_MOD, TK_TIMES, TK_DIV)) {
        short currentPrecedence = Parser::precedence(getCurrentToken());
        if (oldPrecedence > currentPrecedence) {
            return p;
        }
        auto tmp = new BinaryExpr(line, column);
        tmp->lhs = p;
        tmp->opt = getCurrentToken();
        currentToken = next();
        tmp->rhs = parseExpression(currentPrecedence + 1);
        p = tmp;
    }
    return p;
}

ExpressionStmt* Parser::parseExpressionStmt() {
    ExpressionStmt* node = nullptr;
    if (auto p = parseExpression(); p != nullptr) {
        node = new ExpressionStmt(p, line, column);
    }
    return node;
}

IfStmt* Parser::parseIfStmt() {
    auto* node = new IfStmt(line, column);
    currentToken = next();
    node->cond = parseExpression();
    assert(getCurrentToken() == TK_RPAREN);
    currentToken = next();
    node->block = parseBlock();
    if (getCurrentToken() == KW_ELSE) {
        currentToken = next();
        node->elseBlock = parseBlock();
    }
    return node;
}

WhileStmt* Parser::parseWhileStmt() {
    auto* node = new WhileStmt(line, column);
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
            break;
        default:
            node = parseExpressionStmt();
            break;
    }
    return node;
}

std::vector<Statement*> Parser::parseStatementList() {
    std::vector<Statement*> node;
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

std::vector<std::string> Parser::parseParameterList() {
    std::vector<std::string> node;
    currentToken = next();
    if (getCurrentToken() == TK_RPAREN) {
        currentToken = next();
        return std::move(node);
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

nyx::Function* Parser::parseFuncDef() {
    assert(getCurrentToken() == KW_FUNC);
    currentToken = next();
    auto* node = new nyx::Function;
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

std::tuple<Token, std::string> Parser::next() {
    char c = getNextChar();

    if (c == EOF) {
        return std::make_tuple(TK_EOF, "");
    }
    if (anyone(c, ' ', '\n', '\r', '\t')) {
        while (anyone(c, ' ', '\n', '\r', '\t')) {
            if (c == '\n') {
                line++;
                column = 0;
            }
            c = getNextChar();
        }
        if (c == EOF) {
            return std::make_tuple(TK_EOF, "");
        }
    }

    if (c == '#') {
        while (c != '\n' && c != EOF) {
            c = getNextChar();
        }
        // consume newlines
        while (c == '\n') {
            line++;
            column = 0;
            c = getNextChar();
        }
        if (c == EOF) {
            return std::make_tuple(TK_EOF, "");
        }
    }

    if (c >= '0' && c <= '9') {
        std::string lexeme{c};
        bool isDouble = false;
        char cn = peekNextChar();
        while ((cn >= '0' && cn <= '9') || (!isDouble && cn == '.')) {
            if (c == '.') {
                isDouble = true;
            }
            c = getNextChar();
            cn = peekNextChar();
            lexeme += c;
        }
        return !isDouble ? make_tuple(LIT_INT, lexeme)
                         : make_tuple(LIT_DOUBLE, lexeme);
    }
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        std::string lexeme{c};
        char cn = peekNextChar();
        while ((cn >= 'a' && cn <= 'z') || (cn >= 'A' && cn <= 'Z') ||
               (cn >= '0' && cn <= '9')) {
            c = getNextChar();
            lexeme += c;
            cn = peekNextChar();
        }
        auto result = keywords.find(lexeme);
        return result != keywords.end()
                   ? std::make_tuple(result->second, lexeme)
                   : std::make_tuple(TK_IDENT, lexeme);
    }
    if (c == '"') {
        std::string lexeme;
        char cn = peekNextChar();
        while (cn != '"') {
            c = getNextChar();
            lexeme += c;
            cn = peekNextChar();
        }
        getNextChar();
        return std::make_tuple(LIT_STR, lexeme);
    }
    if (c == '[') {
        return std::make_tuple(TK_LBRACKET, "[");
    }
    if (c == ']') {
        return std::make_tuple(TK_RBRACKET, "]");
    }
    if (c == '{') {
        return std::make_tuple(TK_LBRACE, "{");
    }
    if (c == '}') {
        return std::make_tuple(TK_RBRACE, "}");
    }
    if (c == '(') {
        return std::make_tuple(TK_LPAREN, "(");
    }
    if (c == ')') {
        return std::make_tuple(TK_RPAREN, ")");
    }
    if (c == ',') {
        return std::make_tuple(TK_COMMA, ",");
    }
    if (c == '+') {
        return std::make_tuple(TK_PLUS, "+");
    }
    if (c == '-') {
        return std::make_tuple(TK_MINUS, "-");
    }
    if (c == '*') {
        return std::make_tuple(TK_TIMES, "*");
    }
    if (c == '/') {
        return std::make_tuple(TK_DIV, "/");
    }
    if (c == '%') {
        return std::make_tuple(TK_MOD, "%");
    }
    if (c == '~') {
        return std::make_tuple(TK_BITNOT, "~");
    }
    if (c == '=') {
        if (peekNextChar() == '=') {
            c = getNextChar();
            return std::make_tuple(TK_EQ, "==");
        }
        return std::make_tuple(TK_ASSIGN, "=");
    }
    if (c == '!') {
        if (peekNextChar() == '=') {
            c = getNextChar();
            return std::make_tuple(TK_NE, "!=");
        }
        return std::make_tuple(TK_LOGNOT, "!");
    }
    if (c == '|') {
        if (peekNextChar() == '|') {
            c = getNextChar();
            return std::make_tuple(TK_LOGOR, "||");
        }
        return std::make_tuple(TK_BITOR, "|");
    }
    if (c == '&') {
        if (peekNextChar() == '&') {
            c = getNextChar();
            return std::make_tuple(TK_LOGAND, "&&");
        }
        return std::make_tuple(TK_BITAND, "&");
    }
    if (c == '>') {
        if (peekNextChar() == '=') {
            c = getNextChar();
            return std::make_tuple(TK_GE, ">=");
        }
        return std::make_tuple(TK_GT, ">");
    }
    if (c == '<') {
        if (peekNextChar() == '=') {
            c = getNextChar();
            return std::make_tuple(TK_LE, "<=");
        }
        return std::make_tuple(TK_LT, "<");
    }
    panic("SynxaxError: unknown token %c", c);
    return std::make_tuple(INVALID, "invalid");
}

inline short Parser::precedence(Token op) {
    switch (op) {
        case TK_LOGOR:
            return 1;
        case TK_LOGAND:
            return 2;
        case TK_EQ:
        case TK_NE:
        case TK_GT:
        case TK_GE:
        case TK_LT:
        case TK_LE:
            return 3;
        case TK_PLUS:
        case TK_MINUS:
        case TK_BITOR:
            return 4;
        case TK_TIMES:
        case TK_MOD:
        case TK_DIV:
        case TK_BITAND:
            return 5;
        default:
            // Lowest precedence
            return 0;
    }
}
