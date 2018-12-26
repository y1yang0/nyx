#include "Builtin.h"
#include "Nyx.hpp"
#include "Utils.hpp"

namespace nyx {

Context::~Context() {
    for (auto v : vars) {
        delete v.second;
    }
}

Runtime::Runtime() {
    builtin["print"] = &nyx_builtin_print;
    builtin["println"] = &nyx_builtin_println;
    builtin["typeof"] = &nyx_builtin_typeof;
    builtin["input"] = &nyx_builtin_input;
    builtin["length"] = &nyx_builtin_length;
    builtin["to_int"] = &nyx_builtin_to_int;
    builtin["to_double"] = &nyx_builtin_to_double;
    builtin["range"] = &nyx_builtin_range;
}

bool Runtime::hasBuiltinFunction(const std::string& name) {
    return builtin.count(name) == 1;
}

Runtime::BuiltinFuncType Runtime::getBuiltinFunction(const std::string& name) {
    if (auto res = builtin.find(name); res != builtin.end()) {
        return res->second;
    }
    return builtin[name];
}

void Runtime::addStatement(Statement* stmt) { stmts.push_back(stmt); }

std::vector<Statement*>& Runtime::getStatements() { return stmts; }

bool Context::hasVariable(const std::string& identName) {
    return vars.count(identName) == 1;
}

void Context::createVariable(const std::string& identName, Value value) {
    auto* var = new Variable;
    var->name = identName;
    var->value = value;
    vars.emplace(identName, var);
}

Variable* Context::getVariable(const std::string& identName) {
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

Value& Value::operator+(const Value& rhs) {
    // Basic
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = cast<int>() + rhs.cast<int>();
        this->type = nyx::Int;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = cast<double>() + rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        this->data = cast<int>() + rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        this->data = cast<double>() + rhs.cast<int>();
        this->type = nyx::Double;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Int>()) {
        this->data = static_cast<char>(cast<char>() + rhs.cast<int>());
        this->type = nyx::Char;
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Char>()) {
        this->data = static_cast<char>(cast<int>() + rhs.cast<char>());
        this->type = nyx::Char;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = static_cast<char>(cast<char>() + rhs.cast<char>());
        this->type = nyx::Char;
    }
    // String
    // One of operands has string type, we say the result value was a string
    else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        this->data = valueToStdString(*this) + valueToStdString(rhs);
        this->type = nyx::String;
    }
    // Array
    else if (isType<nyx::Array>()) {
        auto resultArr = this->cast<std::vector<nyx::Value>>();
        resultArr.push_back(rhs);
        this->data = resultArr;
        this->type = nyx::Array;
    } else if (rhs.isType<nyx::Array>()) {
        auto resultArr = rhs.cast<std::vector<nyx::Value>>();
        resultArr.push_back(*this);
        this->data = resultArr;
        this->type = nyx::Array;
    }
    // Invalid
    else {
        panic("TypeError: unexpected arguments of operator +");
    }
    return *this;
}

Value& Value::operator-(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = cast<int>() - rhs.cast<int>();
        this->type = nyx::Int;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = cast<double>() - rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        this->data = cast<int>() - rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        this->data = cast<double>() - rhs.cast<int>();
        this->type = nyx::Double;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Int>()) {
        this->data = static_cast<char>(cast<char>() - rhs.cast<int>());
        this->type = nyx::Char;
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Char>()) {
        this->data = static_cast<char>(cast<int>() - rhs.cast<char>());
        this->type = nyx::Char;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = static_cast<char>(cast<char>() - rhs.cast<char>());
        this->type = nyx::Char;
    } else {
        panic("TypeError: unexpected arguments of operator -");
    }

    return *this;
}

Value& Value::operator*(const Value& rhs) {
    // Basic
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = cast<int>() * rhs.cast<int>();
        this->type = nyx::Int;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = cast<double>() * rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        this->data = cast<int>() * rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        this->data = cast<double>() * rhs.cast<int>();
        this->type = nyx::Double;
    }
    // String
    else if (isType<nyx::String>() && rhs.isType<nyx::Int>()) {
        this->data = repeatString(rhs.cast<int>(), cast<std::string>());
        this->type = nyx::String;
    } else if (isType<nyx::Int>() && rhs.isType<nyx::String>()) {
        this->data = repeatString(cast<int>(), rhs.cast<std::string>());
        this->type = nyx::String;
    }
    // Invalid
    else {
        panic("TypeError: unexpected arguments of operator *");
    }
    return *this;
}

Value& Value::operator/(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = cast<int>() / rhs.cast<int>();
        this->type = nyx::Int;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = cast<double>() / rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        this->data = cast<int>() / rhs.cast<double>();
        this->type = nyx::Double;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        this->data = cast<double>() / rhs.cast<int>();
        this->type = nyx::Double;
    } else {
        panic("TypeError: unexpected arguments of operator /");
    }
    return *this;
}

Value& Value::operator%(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (int)cast<int>() % rhs.cast<int>();
        this->type = nyx::Int;
    } else {
        panic("TypeError: unexpected arguments of operator %");
    }
    return *this;
}

Value& Value::operator&&(const Value& rhs) {
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        this->data = (cast<bool>() && rhs.cast<bool>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator &&");
    }
    return *this;
}

Value& Value::operator||(const Value& rhs) {
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        this->data = (cast<bool>() || rhs.cast<bool>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator ||");
    }
    return *this;
}

Value& Value::operator==(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() == rhs.cast<int>());
        this->type = nyx::Bool;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = (cast<double>() == rhs.cast<double>());
        this->type = nyx::Bool;
    } else if (isType<nyx::String>() && rhs.isType<nyx::String>()) {
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        this->data = (lhsStr == rhsStr);
        this->type = nyx::Bool;
    } else if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        this->data = (cast<bool>() == rhs.cast<bool>());
        this->type = nyx::Bool;
    } else if (this->type == nyx::Null && rhs.type == nyx::Null) {
        this->data = std::make_any<bool>(true);
        this->type = nyx::Bool;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = (cast<char>() == rhs.cast<char>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator ==");
    }
    return *this;
}

Value& Value::operator!=(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() != rhs.cast<int>());
        this->type = nyx::Bool;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = (cast<double>() != rhs.cast<double>());
        this->type = nyx::Bool;
    } else if (isType<nyx::String>() && rhs.isType<nyx::String>()) {
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        this->data = (lhsStr != rhsStr);
        this->type = nyx::Bool;
    } else if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        this->data = (cast<bool>() != rhs.cast<bool>());
        this->type = nyx::Bool;
    } else if (this->type == nyx::Null && rhs.type == nyx::Null) {
        this->data = std::make_any<bool>(false);
        this->type = nyx::Bool;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = (cast<char>() != rhs.cast<char>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator !=");
    }
    return *this;
}

Value& Value::operator>(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() > rhs.cast<int>());
        this->type = nyx::Bool;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = (cast<double>() > rhs.cast<double>());
        this->type = nyx::Bool;
    } else if (isType<nyx::String>() && rhs.isType<nyx::String>()) {
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        this->data = (lhsStr > rhsStr);
        this->type = nyx::Bool;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = (cast<char>() > rhs.cast<char>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator >");
    }
    return *this;
}

Value& Value::operator>=(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() >= rhs.cast<int>());
        this->type = nyx::Bool;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = (cast<double>() >= rhs.cast<double>());
        this->type = nyx::Bool;
    } else if (isType<nyx::String>() && rhs.isType<nyx::String>()) {
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        this->data = (lhsStr >= rhsStr);
        this->type = nyx::Bool;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = (cast<char>() >= rhs.cast<char>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator >=");
    }
    return *this;
}

Value& Value::operator<(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() < rhs.cast<int>());
        this->type = nyx::Bool;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = (cast<double>() < rhs.cast<double>());
        this->type = nyx::Bool;
    } else if (isType<nyx::String>() && rhs.isType<nyx::String>()) {
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        this->data = (lhsStr < rhsStr);
        this->type = nyx::Bool;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = (cast<char>() < rhs.cast<char>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator <");
    }
    return *this;
}

Value& Value::operator<=(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() <= rhs.cast<int>());
        this->type = nyx::Bool;
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        this->data = (cast<double>() <= rhs.cast<double>());
        this->type = nyx::Bool;
    } else if (isType<nyx::String>() && rhs.isType<nyx::String>()) {
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        this->data = (lhsStr <= rhsStr);
        this->type = nyx::Bool;
    } else if (isType<nyx::Char>() && rhs.isType<nyx::Char>()) {
        this->data = (cast<char>() <= rhs.cast<char>());
        this->type = nyx::Bool;
    } else {
        panic("TypeError: unexpected arguments of operator <=");
    }
    return *this;
}

Value& Value::operator&(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() & rhs.cast<int>());
        this->type = nyx::Int;
    } else {
        panic("TypeError: unexpected arguments of operator &");
    }
    return *this;
}

Value& Value::operator|(const Value& rhs) {
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        this->data = (cast<int>() | rhs.cast<int>());
        this->type = nyx::Int;
    } else {
        panic("TypeError: unexpected arguments of operator |");
    }
    return *this;
}

}  // namespace nyx
