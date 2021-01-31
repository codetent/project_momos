#include "momos/macros.hpp"

#include <stdio.h>
extern "C"
{
#include "logic.h"
#include "messages.h"
#include "../src/states.c"
}

STATE(uint8_t)
{
    return current_state;
}

HOOK(init)
{
    logic_init();
}

HOOK(update)
{
    logic_run();
}
