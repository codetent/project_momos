#include "states.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define STATE_WAIT 1U              // @state STATE_WAIT [initial]
#define STATE_SEND 2U              // @state STATE_SEND
#define STATE_SEND_TIMESTAMP 3U    // @state STATE_SEND_TIMESTAMP
#define STATE_RECEIVE 4U           // @state STATE_RECEIVE
#define STATE_RECEIVE_TIMESTAMP 5U // @state STATE_RECEIVE_TIMESTAMP

static uint8_t current_state;
static time_t next_time;
static bool received;

void states_init(void)
{
    next_time = time(0) + 2U;
    current_state = STATE_WAIT; // @transition . -> STATE_WAIT
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
            current_state = STATE_SEND; // @transition STATE_WAIT -> STATE_SEND [timeout, value=2]
        }
        break;

    case STATE_SEND:
        // send message
        if (true)
        {
            current_state = STATE_SEND_TIMESTAMP; // @transition STATE_SEND -> STATE_SEND_TIMESTAMP [transmit]
        }
        break;

    case STATE_SEND_TIMESTAMP:
        // get send timestamp
        if (true)
        {
            current_state = STATE_RECEIVE; // @transition STATE_SEND_TIMESTAMP -> STATE_RECEIVE
        }
        break;

    case STATE_RECEIVE:
        if (received)
        {
            received = false;
            current_state = STATE_RECEIVE_TIMESTAMP; // @transition STATE_RECEIVE -> STATE_RECEIVE_TIMESTAMP [receive]
        }
        break;

    case STATE_RECEIVE_TIMESTAMP:
        // get receive timestamp
        if (true)
        {
            next_time = time(0) + 2U;
            current_state = STATE_WAIT; // @transition STATE_RECEIVE_TIMESTAMP -> STATE_WAIT
        }
        break;
    }
}
