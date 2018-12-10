#include "Builtin.h"
#include "Nyx.h"

GlobalContext::GlobalContext() { builtin["print"] = &print; }
