#include <cstdarg>
#include "Nyx.hpp"
#include "Utils.hpp"

std::string valueToStdString(const nyx::Value& v) {
    switch (v.type) {
        case nyx::Bool:
            return v.cast<bool>() ? "true" : "false";
        case nyx::Double:
            return std::to_string(v.cast<double>());
        case nyx::Int:
            return std::to_string(v.cast<int>());
        case nyx::Null:
            return "null";
        case nyx::String:
            return v.cast<std::string>();
        case nyx::Char: {
            std::string str;
            str += v.cast<char>();
            return str;
        }
        case nyx::Array: {
            std::string str = "Array[";
            auto elements = v.cast<std::vector<nyx::Value>>();
            for (int i = 0; i < elements.size(); i++) {
                str += valueToStdString(elements[i]);

                if (i != elements.size() - 1) {
                    str += ",";
                }
            }
            str += "]";
            return str;
        }
        case nyx::Closure: {
            return "closure";
        }
    }
    return "unknown";
}

std::string repeatString(int count, const std::string& str) {
    std::string result;
    for (int i = 0; i < count; i++) {
        result += str;
    }
    return result;
}

std::vector<nyx::Value> repeatArray(int count, std::vector<nyx::Value>&& arr) {
    std::vector<nyx::Value> result;
    for (int i = 0; i < count; i++) {
        result.insert(result.begin(), arr.begin(), arr.end());
    }
    return result;
}

[[noreturn]] void panic(char const* const format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}

bool equalValue(const nyx::Value& a, const nyx::Value& b) {
    if (a.type != b.type) {
        return false;
    }
    switch (a.type) {
        case nyx::Bool:
            return a.cast<bool>() == b.cast<bool>();
        case nyx::Double:
            return a.cast<double>() == b.cast<double>();
        case nyx::Int:
            return a.cast<int>() == b.cast<int>();
        case nyx::Null:
            return true;
        case nyx::String:
            return a.cast<std::string>() == b.cast<std::string>();
        case nyx::Char:
            return a.cast<char>() == b.cast<char>();
        case nyx::Array: {
            auto elements1 = a.cast<std::vector<nyx::Value>>();
            auto elements2 = b.cast<std::vector<nyx::Value>>();
            if (elements1.size() != elements2.size()) {
                return false;
            }
            for (int i = 0; i < elements1.size(); i++) {
                if (!equalValue(elements1[i], elements2[i])) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}
