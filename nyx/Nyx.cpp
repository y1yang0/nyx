#include "Builtin.h"
#include "Nyx.h"

nyx::GlobalContext::GlobalContext() {
    builtin["print"] = &nyx_builtin_print;
    builtin["typeof"] = &nyx_builtin_typeof;
}
