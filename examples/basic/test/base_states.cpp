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

TRANSITION(STATE_RECEIVING, STATE_SENDING)
{
    uint32_t value = 12;
    io::in.provide(&value, sizeof(value), 0);
}

TRANSITION(STATE_SENDING, STATE_RECEIVING)
{
    printf("value: %f\n", FLOAT_ARG);
    sleep(FLOAT_ARG);
}
