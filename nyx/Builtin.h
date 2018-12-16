#pragma once

#include <deque>
#include <vector>
#include "Nyx.hpp"

nyx::Value nyx_builtin_print(nyx::Runtime* rt,
                             std::deque<nyx::Context*> ctxChain,
                             std::vector<nyx::Value> args);

nyx::Value nyx_builtin_input(nyx::Runtime* rt,
                             std::deque<nyx::Context*> ctxChain,
                             std::vector<nyx::Value> args);

nyx::Value nyx_builtin_typeof(nyx::Runtime* rt,
                              std::deque<nyx::Context*> ctxChain,
                              std::vector<nyx::Value> args);

nyx::Value nyx_builtin_length(nyx::Runtime* rt,
                              std::deque<nyx::Context*> ctxChain,
                              std::vector<nyx::Value> args);
