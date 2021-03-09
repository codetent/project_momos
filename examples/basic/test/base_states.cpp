#include <stdio.h>
#include <unistd.h>

#include <gmock/gmock.h>
#include <momos.hpp>

#include "mocks/hal_mock.hpp"

extern "C"
{
#include "../src/states.c"
}

using namespace ::testing;

hal_mock hal_mock_obj;

/* --------------------------------- States --------------------------------- */

STATE(WAIT, STATE_WAIT);
STATE(SEND, STATE_SEND);
STATE(SEND_TIMESTAMP, STATE_SEND_TIMESTAMP);
STATE(RECEIVE, STATE_RECEIVE);
STATE(RECEIVE_TIMESTAMP, STATE_RECEIVE_TIMESTAMP);

STATE_VAR(current_state);

/* ---------------------------------- Hooks --------------------------------- */

HOOK(before)
{
    states_init();
}

HOOK(process)
{
    states_run();
}

/* ------------------------------- Transitions ------------------------------ */

PREPARE(WAIT, SEND)
{
    sleep(2 * INT_ARG);
}

PREPARE(SEND, SEND_TIMESTAMP)
{
    EXPECT_CALL(hal_mock_obj, transmit(42)).Times(1);
}

PREPARE(RECEIVE, RECEIVE_TIMESTAMP)
{
    states_update(42);
}

PREPARE(RECEIVE, WAIT, receive)
{
    states_update(0);
}

PREPARE(RECEIVE, WAIT, timeout)
{
    sleep(3 * INT_ARG);
}
