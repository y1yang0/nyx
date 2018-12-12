#include "Nyx.h"
#include "Utils.h"

std::string valueToStdString(nyx::Value v) {
    switch (v.type) {
        case nyx::NyxBool:
            return std::any_cast<bool>(v.data) ? "true" : "false";
        case nyx::NyxDouble:
            return std::to_string(std::any_cast<double>(v.data));
        case nyx::NyxInt:
            return std::to_string(std::any_cast<int>(v.data));
        case nyx::NyxNull:
            return "null";
        case nyx::NyxString:
            return std::any_cast<std::string>(v.data);
    }
    return "unknow";
}
