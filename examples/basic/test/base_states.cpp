#include <stdio.h>
#include <unistd.h>

#include <gmock/gmock.h>

#include "momos.hpp"
#include "mocks/hal_mock.hpp"

extern "C"
{
#include "logic.h"
#include "messages.h"
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

HOOK(progress)
{
    states_run();
}

/* ------------------------------- Transitions ------------------------------ */

PREPARE(WAIT, SEND)
{
    sleep(2.0 * FLOAT_ARG);
}

PREPARE(RECEIVE, RECEIVE_TIMESTAMP)
{
    states_update();
}

PREPARE(SEND, SEND_TIMESTAMP)
{
    EXPECT_CALL(hal_mock_obj, transmit(42)).Times(1);
}
