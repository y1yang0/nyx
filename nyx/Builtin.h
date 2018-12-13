#pragma once

#include <vector>
#include "Nyx.h"

nyx::Value nyx_builtin_print(nyx::GlobalContext* ctx,
                             std::vector<nyx::Value> args);

nyx::Value nyx_builtin_input(nyx::GlobalContext* ctx,
                             std::vector<nyx::Value> args);

nyx::Value nyx_builtin_typeof(nyx::GlobalContext* ctx,
                              std::vector<nyx::Value> args);
