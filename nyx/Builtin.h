#pragma once

#include <deque>
#include <vector>
#include "Nyx.hpp"

nyx::Value nyx_builtin_print(nyx::NyxContext* nyxCtx,
                             std::deque<nyx::Context*> ctxChain,
                             std::vector<nyx::Value> args);

nyx::Value nyx_builtin_input(nyx::NyxContext* nyxCtx,
                             std::deque<nyx::Context*> ctxChain,
                             std::vector<nyx::Value> args);

nyx::Value nyx_builtin_typeof(nyx::NyxContext* nyxCtx,
                              std::deque<nyx::Context*> ctxChain,
                              std::vector<nyx::Value> args);
