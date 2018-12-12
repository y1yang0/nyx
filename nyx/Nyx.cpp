#include "Builtin.h"
#include "Nyx.h"

nyx::GlobalContext::GlobalContext() { builtin["print"] = &print; }
