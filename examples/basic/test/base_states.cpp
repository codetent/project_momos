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

hal_mock *hal_mock_obj;

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
    hal_mock_obj = new hal_mock();

    EXPECT_CALL(*hal_mock_obj, current_time)
        .WillOnce(Return(0));

    states_init();
}

HOOK(process)
{
    states_run();
}

HOOK(after)
{
    delete hal_mock_obj;
}

/* ------------------------------- Transitions ------------------------------ */

PREPARE(WAIT, SEND)
{
    EXPECT_CALL(*hal_mock_obj, current_time)
        .WillOnce(Return(2 * INT_ARG));
}

PREPARE(SEND, SEND_TIMESTAMP)
{
    EXPECT_CALL(*hal_mock_obj, transmit(42))
        .Times(1);
}

PREPARE(SEND_TIMESTAMP, RECEIVE)
{
    EXPECT_CALL(*hal_mock_obj, current_time)
        .WillOnce(Return(0));
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
    uint32_t time = 2 * INT_ARG;

    if (time > next_time)
    {
        EXPECT_CALL(*hal_mock_obj, current_time)
            .WillOnce(Return(time))
            .WillOnce(Return(time));
    }
    else
    {
        EXPECT_CALL(*hal_mock_obj, current_time)
            .WillOnce(Return(time));
    }
}

PREPARE(RECEIVE_TIMESTAMP, WAIT)
{
    EXPECT_CALL(*hal_mock_obj, current_time)
        .WillOnce(Return(0));
}
