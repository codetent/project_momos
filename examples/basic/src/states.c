#include "states.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "hal.h"
#include "messages.h"

#define STATE_WAIT 1U              // @state WAIT [initial]
#define STATE_SEND 2U              // @state SEND
#define STATE_SEND_TIMESTAMP 3U    // @state SEND_TIMESTAMP
#define STATE_RECEIVE 4U           // @state RECEIVE
#define STATE_RECEIVE_TIMESTAMP 5U // @state RECEIVE_TIMESTAMP

static uint8_t current_state;
static time_t next_time;
static bool received;

void states_init(void)
{
    next_time = time(0) + 2U;
    current_state = STATE_WAIT; // @transition . -> WAIT
    received = false;
}

void states_update(void)
{
    received = true;
}

void states_run(void)
{
    switch (current_state)
    {
    case STATE_WAIT:
        if (time(0) >= next_time)
        {
            current_state = STATE_SEND; // @transition WAIT -> SEND [timeout]
        }
        break;

    case STATE_SEND:
        send_message(42);

        if (true)
        {
            current_state = STATE_SEND_TIMESTAMP; // @transition SEND -> SEND_TIMESTAMP [transmit]
        }
        break;

    case STATE_SEND_TIMESTAMP:
        // get send timestamp
        if (true)
        {
            current_state = STATE_RECEIVE; // @transition SEND_TIMESTAMP -> RECEIVE
        }
        break;

    case STATE_RECEIVE:
        if (received)
        {
            received = false;
            current_state = STATE_RECEIVE_TIMESTAMP; // @transition RECEIVE -> RECEIVE_TIMESTAMP [receive]
        }
        break;

    case STATE_RECEIVE_TIMESTAMP:
        // get receive timestamp
        if (true)
        {
            next_time = time(0) + 2U;
            current_state = STATE_WAIT; // @transition RECEIVE_TIMESTAMP -> WAIT
        }
        break;
    }
}
