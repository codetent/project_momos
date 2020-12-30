#include "logic.h"

#include "states.h"
#include "hal.h"

#include <stdio.h>

void logic_init(void)
{
    states_init();
}

void logic_run(void)
{
    int received;
    bool available;

    available = receive(&received);

    if (available && received == 12)
    {
        states_update();
    }

    states_run();
}
