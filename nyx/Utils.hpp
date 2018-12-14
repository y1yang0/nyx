#pragma once
#include <any>
#include <deque>
#include <string>
#include "Nyx.hpp"

std::string valueToStdString(nyx::Value v);

template <typename _DesireType, typename... _ArgumentType>
inline bool anyone(_DesireType k, _ArgumentType... args) {
    return ((args == k) || ...);
}

[[noreturn]] void panic(char const* const format, ...);
