#pragma once

#include <string>
#include <vector>

using namespace std;

class Value {
public:
    explicit Value() {}

private:
};

class Variable {
public:
    explicit Variable() {}

private:
    string name;
    Value value;
};

class Statement {
public:
    explicit Statement() {}

private:
};

class Block {
public:
    explicit Block() {}

private:
    vector<Statement> stmts;
};

class Function {
public:
    explicit Function() {}

private:
    string name;
    vector<string> params;
    Block block;
};

class NyxInterpreter {
public:
    explicit NyxInterpreter() {}

private:
    vector<Variable> vars;
    vector<Function> funcDefs;
};
