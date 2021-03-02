#include <stdio.h>
#include <unistd.h>

#include <gmock/gmock.h>

#include "momos/macros.hpp"
#include "mocks/hal_mock.hpp"

extern "C"
{
#include "logic.h"
#include "messages.h"
#include "../src/states.c"
}

using namespace ::testing;

hal_mock hal_mock_obj;

STATE(uint8_t)
{
    return current_state;
}

/* ---------------------------------- Hooks --------------------------------- */

HOOK(before_all)
{
    EXPECT_CALL(hal_mock_obj, receive)
        .WillRepeatedly(Return(false));
}

HOOK(before)
{
    states_init();
}

HOOK(progress)
{
    states_run();
}

/* ------------------------------- Transitions ------------------------------ */

PREPARE(STATE_WAIT, STATE_SEND)
{
    sleep(2.0 * FLOAT_ARG);
}

PREPARE(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP)
{
    states_update();
}

PREPARE(STATE_SEND, STATE_SEND_TIMESTAMP)
{
    EXPECT_CALL(hal_mock_obj, transmit(42));
}
