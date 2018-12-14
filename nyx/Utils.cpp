#include <cstdarg>
#include "Nyx.hpp"
#include "Utils.hpp"

std::string valueToStdString(nyx::Value v) {
    switch (v.type) {
        case nyx::Bool:
            return std::any_cast<bool>(v.data) ? "true" : "false";
        case nyx::Double:
            return std::to_string(std::any_cast<double>(v.data));
        case nyx::Int:
            return std::to_string(std::any_cast<int>(v.data));
        case nyx::Null:
            return "null";
        case nyx::String:
            return std::any_cast<std::string>(v.data);
    }
    return "unknown";
}

[[noreturn]] void panic(char const* const format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
