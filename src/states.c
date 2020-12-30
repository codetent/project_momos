#include "states.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define STATE_RECEIVING 1U
#define STATE_SENDING 2U

static uint8_t current_state;
static time_t next_time;

void states_init(void)
{
    current_state = STATE_RECEIVING;
}

void states_update(void)
{
    // @transition STATE_RECEIVING -> STATE_SENDING { trigger: {type: "receive", builder: "messages.h:send_message(7)", count_sensitive: false} }
    current_state = STATE_SENDING;
}

void states_run(void)
{
    switch (current_state)
    {
    case STATE_RECEIVING: // @state STATE_RECEIVING {initial: true}
        printf("state: receiving\n");
        next_time = time(0) + 2U;
        break;

    case STATE_SENDING: // @state STATE_SENDING
        printf("state: sending\n");

        if (time(0) >= next_time)
        {
            current_state = STATE_RECEIVING; // @transition STATE_SENDING -> STATE_RECEIVING { trigger: {type: "timeout", value: 2} }
        }
        break;
    }
}
