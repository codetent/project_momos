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
static int received_msg;

void states_init(void)
{
    next_time = time(0) + 2U;
    current_state = STATE_WAIT; // @transition . -> WAIT
    received = false;
    received_msg = 0U;
}

void states_update(int msg)
{
    received = true;
    received_msg = msg;
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
        current_state = STATE_SEND_TIMESTAMP; // @transition SEND -> SEND_TIMESTAMP [send]
        break;

    case STATE_SEND_TIMESTAMP:
        current_state = STATE_RECEIVE; // @transition SEND_TIMESTAMP -> RECEIVE
        next_time = time(0) + 2U;
        break;

    case STATE_RECEIVE:
        if (received)
        {
            received = false;

            if (received_msg == 42)
            {
                current_state = STATE_RECEIVE_TIMESTAMP; // @transition RECEIVE -> RECEIVE_TIMESTAMP [receive#correct, max_count=1]
            }
            else
            {
                current_state = STATE_WAIT; // @transition RECEIVE -> WAIT [receive#incorrect, max_count=1]
            }
        }
        else if (time(0) > next_time)
        {
            next_time = time(0) + 2U;
            current_state = STATE_WAIT; // @transition RECEIVE -> WAIT [timeout]
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
