#include <cstdarg>
#include "Nyx.hpp"
#include "Utils.hpp"

std::string valueToStdString(nyx::Value v) {
    switch (v.type) {
        case nyx::Bool:
            return v.cast<bool>() ? "true" : "false";
        case nyx::Double:
            return std::to_string(v.cast<double>());
        case nyx::Int:
            return std::to_string(v.cast<int>());
        case nyx::Null:
            return "null";
        case nyx::Char: {
            std::string str;
            str += v.cast<char>();
            return str;
        }
        case nyx::String:
            return v.cast<std::string>();
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

[[noreturn]] void panic(char const* const format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
