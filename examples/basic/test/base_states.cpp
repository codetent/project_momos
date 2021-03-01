#include "momos/macros.hpp"

#include <stdio.h>
#include <unistd.h>

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

TRANSITION(STATE_WAIT, STATE_SEND)
{
    sleep(FLOAT_ARG);
}

TRANSITION(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP)
{
    uint32_t value = 12;
    io::in.provide(&value, sizeof(value), 0);
}
