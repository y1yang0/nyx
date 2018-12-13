#include "Builtin.h"
#include "Nyx.hpp"
#include "Utils.h"

nyx::GlobalContext::GlobalContext() {
    builtin["print"] = &nyx_builtin_print;
    builtin["typeof"] = &nyx_builtin_typeof;
    builtin["input"] = &nyx_builtin_input;
}

nyx::Value nyx::Value::operator+(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data =
            std::any_cast<int>(this->data) + std::any_cast<int>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) + std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<int>(this->data) + std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) + std::any_cast<int>(rhs.data);
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::String;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = lhsStr + rhsStr;
    } else {
        throw std::runtime_error("unexpected arguments of +");
    }
    return result;
}

nyx::Value nyx::Value::operator-(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data =
            std::any_cast<int>(this->data) - std::any_cast<int>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) - std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<int>(this->data) - std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) - std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error("unexpected arguments of -");
    }

    return result;
}

nyx::Value nyx::Value::operator*(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data =
            std::any_cast<int>(this->data) * std::any_cast<int>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) * std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<int>(this->data) * std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) * std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error("unexpected arguments of *");
    }
    return result;
}

nyx::Value nyx::Value::operator/(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data =
            std::any_cast<int>(this->data) / std::any_cast<int>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) / std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Int>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<int>(this->data) / std::any_cast<double>(rhs.data);
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Double;
        result.data =
            std::any_cast<double>(this->data) / std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error("unexpected arguments of /");
    }
    return result;
}

nyx::Value nyx::Value::operator%(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data =
            (int)std::any_cast<int>(this->data) % std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error("unexpected arguments of %");
    }
    return result;
}

nyx::Value nyx::Value::operator&&(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data =
            (std::any_cast<bool>(this->data) && std::any_cast<bool>(rhs.data));
    } else {
        throw std::runtime_error("unexpected arguments of &&");
    }
    return result;
}

nyx::Value nyx::Value::operator||(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data =
            (std::any_cast<bool>(this->data) || std::any_cast<bool>(rhs.data));
    } else {
        throw std::runtime_error("unexpected arguments of ||");
    }
    return result;
}

nyx::Value nyx::Value::operator==(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Bool;
        result.data =
            (std::any_cast<int>(this->data) == std::any_cast<int>(rhs.data));
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (std::any_cast<double>(this->data) ==
                       std::any_cast<double>(rhs.data));
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr == rhsStr);
    } else if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data =
            (std::any_cast<bool>(this->data) == std::any_cast<bool>(rhs.data));
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
        result.data =
            (std::any_cast<int>(this->data) != std::any_cast<int>(rhs.data));
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (std::any_cast<double>(this->data) !=
                       std::any_cast<double>(rhs.data));
    } else if (isType<nyx::String>() || rhs.isType<nyx::String>()) {
        result.type = nyx::Bool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr != rhsStr);
    } else if (isType<nyx::Bool>() && rhs.isType<nyx::Bool>()) {
        result.type = nyx::Bool;
        result.data =
            (std::any_cast<bool>(this->data) != std::any_cast<bool>(rhs.data));
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
        result.data =
            (std::any_cast<int>(this->data) > std::any_cast<int>(rhs.data));
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (std::any_cast<double>(this->data) >
                       std::any_cast<double>(rhs.data));
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
        result.data =
            (std::any_cast<int>(this->data) >= std::any_cast<int>(rhs.data));
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (std::any_cast<double>(this->data) >=
                       std::any_cast<double>(rhs.data));
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
        result.data =
            (std::any_cast<int>(this->data) < std::any_cast<int>(rhs.data));
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (std::any_cast<double>(this->data) <
                       std::any_cast<double>(rhs.data));
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
        result.data =
            (std::any_cast<int>(this->data) <= std::any_cast<int>(rhs.data));
    } else if (isType<nyx::Double>() && rhs.isType<nyx::Double>()) {
        result.type = nyx::Bool;
        result.data = (std::any_cast<double>(this->data) <=
                       std::any_cast<double>(rhs.data));
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
        result.data =
            (std::any_cast<int>(this->data) & std::any_cast<int>(rhs.data));
    } else {
        throw std::runtime_error("unexpected arguments of &");
    }
    return result;
}

nyx::Value nyx::Value::operator|(nyx::Value rhs) {
    nyx::Value result;
    if (isType<nyx::Int>() && rhs.isType<nyx::Int>()) {
        result.type = nyx::Int;
        result.data =
            (std::any_cast<int>(this->data) | std::any_cast<int>(rhs.data));
    } else {
        throw std::runtime_error("unexpected arguments of |");
    }
    return result;
}
