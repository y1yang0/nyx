#include "Builtin.h"
#include "Nyx.hpp"
#include "Utils.hpp"

namespace nyx {

Context::~Context() {
    for (auto v : vars) {
        delete v.second;
    }
}

nyx::NyxContext::NyxContext() {
    builtin["print"] = &nyx_builtin_print;
    builtin["typeof"] = &nyx_builtin_typeof;
    builtin["input"] = &nyx_builtin_input;
}

bool nyx::NyxContext::hasBuiltinFunction(const std::string& name) {
    return builtin.count(name) == 1;
}

nyx::NyxContext::BuiltinFuncType nyx::NyxContext::getBuiltinFunction(
    const std::string& name) {
    if (auto res = builtin.find(name); res != builtin.end()) {
        return res->second;
    }
    return builtin[name];
}

void nyx::NyxContext::addStatement(Statement* stmt) { stmts.push_back(stmt); }

std::vector<Statement*> nyx::NyxContext::getStatements() { return stmts; }

bool Context::removeVariable(const std::string& identName) {
    auto* found = getVariable(identName);
    delete found;
    return vars.erase(identName);
}

bool Context::hasVariable(const std::string& identName) {
    return vars.count(identName) == 1;
}

void Context::addVariable(const std::string& identName, Value value) {
    auto* var = new nyx::Variable;
    var->name = identName;
    var->value = value;
    vars.emplace(identName, var);
}

nyx::Variable* Context::getVariable(const std::string& identName) {
    if (auto res = vars.find(identName); res != vars.end()) {
        return res->second;
    }
    return nullptr;
}

void Context::addFunction(const std::string& name, Function* f) {
    funcs.insert(std::make_pair(name, f));
}

bool Context::hasFunction(const std::string& name) {
    return funcs.count(name) == 1;
}

Function* Context::getFunction(const std::string& name) {
    if (auto f = funcs.find(name); f != funcs.end()) {
        return f->second;
    }
    return nullptr;
}

Value Value::operator+(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = value_cast<int>() + rhs.value_cast<int>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() + rhs.value_cast<double>();
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<int>() + rhs.value_cast<double>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() + rhs.value_cast<int>();
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::String;
        result.data = valueToStdString(*this) + valueToStdString(rhs);
    } else {
        panic("TypeError: unexpected arguments of operator +");
    }
    return result;
}

Value Value::operator-(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = value_cast<int>() - rhs.value_cast<int>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() - rhs.value_cast<double>();
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<int>() - rhs.value_cast<double>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() - rhs.value_cast<int>();
    } else {
        panic("TypeError: unexpected arguments of operator -");
    }

    return result;
}

Value Value::operator*(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = value_cast<int>() * rhs.value_cast<int>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() * rhs.value_cast<double>();
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<int>() * rhs.value_cast<double>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() * rhs.value_cast<int>();
    } else {
        panic("TypeError: unexpected arguments of operator *");
    }
    return result;
}

Value Value::operator/(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = value_cast<int>() / rhs.value_cast<int>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() / rhs.value_cast<double>();
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data = value_cast<int>() / rhs.value_cast<double>();
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data = value_cast<double>() / rhs.value_cast<int>();
    } else {
        panic("TypeError: unexpected arguments of operator /");
    }
    return result;
}

Value Value::operator%(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = (int)value_cast<int>() % rhs.value_cast<int>();
    } else {
        panic("TypeError: unexpected arguments of operator %");
    }
    return result;
}

Value Value::operator&&(Value rhs) {
    Value result;
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<bool>() && rhs.value_cast<bool>());
    } else {
        panic("TypeError: unexpected arguments of operator &&");
    }
    return result;
}

Value Value::operator||(Value rhs) {
    Value result;
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<bool>() || rhs.value_cast<bool>());
    } else {
        panic("TypeError: unexpected arguments of operator ||");
    }
    return result;
}

Value Value::operator==(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<int>() == rhs.value_cast<int>());
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<double>() == rhs.value_cast<double>());
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr == rhsStr);
    } else if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<bool>() == rhs.value_cast<bool>());
    } else if (this->type == nyx::Null && rhs.type == nyx::Null) {
        result.type = nyx::Bool;
        result.data = std::make_any<bool>(true);
    } else {
        panic("TypeError: unexpected arguments of operator ==");
    }
    return result;
}

Value Value::operator!=(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<int>() != rhs.value_cast<int>());
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<double>() != rhs.value_cast<double>());
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr != rhsStr);
    } else if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<bool>() != rhs.value_cast<bool>());
    } else if (this->type == nyx::Null && rhs.type == nyx::Null) {
        result.type = nyx::Bool;
        result.data = std::make_any<bool>(false);
    } else {
        panic("TypeError: unexpected arguments of operator !=");
    }
    return result;
}

Value Value::operator>(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<int>() > rhs.value_cast<int>());
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<double>() > rhs.value_cast<double>());
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr > rhsStr);
    } else {
        panic("TypeError: unexpected arguments of operator >");
    }
    return result;
}

Value Value::operator>=(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<int>() >= rhs.value_cast<int>());
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<double>() >= rhs.value_cast<double>());
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr >= rhsStr);
    } else {
        panic("TypeError: unexpected arguments of operator >=");
    }
    return result;
}

Value Value::operator<(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<int>() < rhs.value_cast<int>());
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<double>() < rhs.value_cast<double>());
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr < rhsStr);
    } else {
        panic("TypeError: unexpected arguments of operator <");
    }
    return result;
}

Value Value::operator<=(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<int>() <= rhs.value_cast<int>());
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<double>() <= rhs.value_cast<double>());
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr <= rhsStr);
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return result;
}

Value Value::operator&(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = (value_cast<int>() & rhs.value_cast<int>());
    } else {
        panic("TypeError: unexpected arguments of operator &");
    }
    return result;
}

Value Value::operator|(Value rhs) {
    Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = (value_cast<int>() | rhs.value_cast<int>());
    } else {
        panic("TypeError: unexpected arguments of operator |");
    }
    return result;
}

}  // namespace nyx
