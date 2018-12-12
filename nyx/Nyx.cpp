#include "Builtin.h"
#include "Nyx.h"
#include "Utils.h"

nyx::GlobalContext::GlobalContext() {
    builtin["print"] = &nyx_builtin_print;
    builtin["typeof"] = &nyx_builtin_typeof;
}

nyx::Value nyx::Value::operator+(nyx::Value rhs) {
    nyx::Value result;
    if (this->type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxInt;
        result.data =
            std::any_cast<int>(this->data) + std::any_cast<int>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) + std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxInt && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<int>(this->data) + std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) + std::any_cast<int>(rhs.data);
    } else if (this->type == nyx::NyxString || rhs.type == nyx::NyxString) {
        result.type = nyx::NyxString;
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
    if (this->type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxInt;
        result.data =
            std::any_cast<int>(this->data) - std::any_cast<int>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) - std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxInt && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<int>(this->data) - std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) - std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error("unexpected arguments of -");
    }

    return result;
}

nyx::Value nyx::Value::operator*(nyx::Value rhs) {
    nyx::Value result;
    if (this->type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxInt;
        result.data =
            std::any_cast<int>(this->data) * std::any_cast<int>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) * std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxInt && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<int>(this->data) * std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) * std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error("unexpected arguments of *");
    }
    return result;
}

nyx::Value nyx::Value::operator/(nyx::Value rhs) {
    nyx::Value result;
    if (this->type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxDouble;
        result.data = (double)std::any_cast<int>(this->data) /
                      std::any_cast<int>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) / std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxInt && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<int>(this->data) / std::any_cast<double>(rhs.data);
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxDouble;
        result.data =
            std::any_cast<double>(this->data) / std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error("unexpected arguments of /");
    }
    return result;
}

nyx::Value nyx::Value::operator%(nyx::Value rhs) {
    nyx::Value result;
    if (this->type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxInt;
        result.data =
            (int)std::any_cast<int>(this->data) % std::any_cast<int>(rhs.data);
    } else {
        throw std::runtime_error(
            "the operators % only takes two int arguments");
    }
    return result;
}

nyx::Value nyx::Value::operator&&(nyx::Value rhs) {
    nyx::Value result;
    if (this->type == nyx::NyxBool && rhs.type == nyx::NyxBool) {
        result.type = nyx::NyxBool;
        result.data =
            (std::any_cast<bool>(this->data) && std::any_cast<bool>(rhs.data));
    } else {
        throw std::runtime_error("unexpected arguments of &&");
    }
    return result;
}

nyx::Value nyx::Value::operator||(nyx::Value rhs) {
    nyx::Value result;
    if (this->type == nyx::NyxBool && rhs.type == nyx::NyxBool) {
        result.type = nyx::NyxBool;
        result.data =
            (std::any_cast<bool>(this->data) || std::any_cast<bool>(rhs.data));
    } else {
        throw std::runtime_error("unexpected arguments of &&");
    }
    return result;
}

nyx::Value nyx::Value::operator==(nyx::Value rhs) {
    nyx::Value result;
    if (this->type == nyx::NyxInt && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxBool;
        result.data =
            (std::any_cast<int>(this->data) == std::any_cast<int>(rhs.data));
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxBool;
        result.data = (std::any_cast<double>(this->data) ==
                       std::any_cast<double>(rhs.data));
    } else if (this->type == nyx::NyxInt && rhs.type == nyx::NyxDouble) {
        result.type = nyx::NyxBool;
        result.data =
            (std::any_cast<int>(this->data) == std::any_cast<double>(rhs.data));
    } else if (this->type == nyx::NyxDouble && rhs.type == nyx::NyxInt) {
        result.type = nyx::NyxBool;
        result.data =
            (std::any_cast<double>(this->data) == std::any_cast<int>(rhs.data));
    } else if (this->type == nyx::NyxString || rhs.type == nyx::NyxString) {
        result.type = nyx::NyxBool;
        std::string lhsStr, rhsStr;
        lhsStr = valueToStdString(*this);
        rhsStr = valueToStdString(rhs);
        result.data = (lhsStr == rhsStr);
    } else {
        throw std::runtime_error("unexpected arguments of ==");
    }
    return result;
}
