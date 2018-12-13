#include "Builtin.h"
#include "Nyx.hpp"
#include "Utils.hpp"

nyx::GlobalContext::GlobalContext() {
    builtin["print"] = &nyx_builtin_print;
    builtin["typeof"] = &nyx_builtin_typeof;
    builtin["input"] = &nyx_builtin_input;
}

nyx::Value nyx::Value::operator+(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of +");
    }
    return result;
}

nyx::Value nyx::Value::operator-(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of -");
    }

    return result;
}

nyx::Value nyx::Value::operator*(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of *");
    }
    return result;
}

nyx::Value nyx::Value::operator/(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of /");
    }
    return result;
}

nyx::Value nyx::Value::operator%(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = (int)value_cast<int>() % rhs.value_cast<int>();
    } else {
        throw std::runtime_error("unexpected arguments of %");
    }
    return result;
}

nyx::Value nyx::Value::operator&&(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<bool>() && rhs.value_cast<bool>());
    } else {
        throw std::runtime_error("unexpected arguments of &&");
    }
    return result;
}

nyx::Value nyx::Value::operator||(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data = (value_cast<bool>() || rhs.value_cast<bool>());
    } else {
        throw std::runtime_error("unexpected arguments of ||");
    }
    return result;
}

nyx::Value nyx::Value::operator==(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of ==");
    }
    return result;
}

nyx::Value nyx::Value::operator!=(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of !=");
    }
    return result;
}

nyx::Value nyx::Value::operator>(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of >");
    }
    return result;
}

nyx::Value nyx::Value::operator>=(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of >=");
    }
    return result;
}

nyx::Value nyx::Value::operator<(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of <");
    }
    return result;
}

nyx::Value nyx::Value::operator<=(nyx::Value rhs) {
    nyx::Value result;
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
        throw std::runtime_error("unexpected arguments of <=");
    }
    return result;
}

nyx::Value nyx::Value::operator&(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = (value_cast<int>() & rhs.value_cast<int>());
    } else {
        throw std::runtime_error("unexpected arguments of &");
    }
    return result;
}

nyx::Value nyx::Value::operator|(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data = (value_cast<int>() | rhs.value_cast<int>());
    } else {
        throw std::runtime_error("unexpected arguments of |");
    }
    return result;
}
