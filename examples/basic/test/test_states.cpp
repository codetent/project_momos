#include <gtest/gtest.h>
#include "momos/builder.hpp"
#include "momos/testif.hpp"

extern "C"
{
    #include "unistd.h"
    #include "src/states.c"
    #include "messages.h"
    #include "logic.h"
}

auto &interface = TestInterface<int>::get_instance();

class StateTest : public ::testing::Test
{
protected:
    void SetUp() override {
        interface.reset();

        logic_init();
        logic_run();
    }

    void TearDown() override {
    }
};


TEST_F(StateTest, STATE_RECEIVING_ExpectedIsReceived)
{
    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(current_state, STATE_RECEIVING);

    send_message(7);
    interface.rx_channel.messages.push_back(interface.tx_channel.get_last_message());

    logic_run();

    ASSERT_EQ(current_state, STATE_SENDING);

}

TEST_F(StateTest, STATE_RECEIVING_NoMessageIsReceived)
{
    // [Step 1]: STATE_RECEIVING: No message is received. ----------
    ASSERT_EQ(current_state, STATE_RECEIVING);


    logic_run();

    ASSERT_NE(current_state, STATE_SENDING);

}

TEST_F(StateTest, STATE_SENDING_TimeoutEqualsExpectedValue)
{
    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(current_state, STATE_RECEIVING);

    send_message(7);
    interface.rx_channel.messages.push_back(interface.tx_channel.get_last_message());

    logic_run();

    ASSERT_EQ(current_state, STATE_SENDING);

    // [Step 2]: STATE_SENDING: Timeout equals expected value. ----------
    ASSERT_EQ(current_state, STATE_SENDING);

    sleep(2);
    // no processor found for timeout

    logic_run();

    ASSERT_EQ(current_state, STATE_RECEIVING);

}

TEST_F(StateTest, STATE_SENDING_TimeoutLessThanExpected)
{
    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(current_state, STATE_RECEIVING);

    send_message(7);
    interface.rx_channel.messages.push_back(interface.tx_channel.get_last_message());

    logic_run();

    ASSERT_EQ(current_state, STATE_SENDING);

    // [Step 2]: STATE_SENDING: Timeout less than expected. ----------
    ASSERT_EQ(current_state, STATE_SENDING);

    sleep(0);
    // no processor found for timeout

    logic_run();

    ASSERT_NE(current_state, STATE_RECEIVING);

}

TEST_F(StateTest, STATE_SENDING_TimeoutGreaterThanExpected)
{
    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(current_state, STATE_RECEIVING);

    send_message(7);
    interface.rx_channel.messages.push_back(interface.tx_channel.get_last_message());

    logic_run();

    ASSERT_EQ(current_state, STATE_SENDING);

    // [Step 2]: STATE_SENDING: Timeout greater than expected. ----------
    ASSERT_EQ(current_state, STATE_SENDING);

    sleep(4);
    // no processor found for timeout

    logic_run();

    ASSERT_EQ(current_state, STATE_RECEIVING);

}

