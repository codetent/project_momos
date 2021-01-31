#include <gtest/gtest.h>

#include "momos/io.hpp"
#include "momos/macros.hpp"

#include "base_states.cpp"


class StateTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        io::in.reset();
        io::out.reset();

        HOOK_RUN(init);
        HOOK_RUN(update);
    }

    void TearDown() override
    {
        HOOK_RUN(deinit);
    }
};

TEST_F(StateTest, STATE_RECEIVING_ExpectedIsReceived)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    auto arg_1_1 = 0;
    TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1);
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);

}

TEST_F(StateTest, STATE_RECEIVING_NoMessageIsReceived)
{

    // [Step 1]: STATE_RECEIVING: No message is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);


    ASSERT_NE(STATE_GET(), STATE_SENDING);

}

TEST_F(StateTest, STATE_SENDING_TimeoutEqualsExpectedValue)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    auto arg_1_1 = 0;
    TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1);
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);


    // [Step 2]: STATE_SENDING: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_SENDING);

    auto arg_2_1 = 2;
    TRANSITION_RUN(STATE_SENDING, STATE_RECEIVING, &arg_2_1);
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

}

TEST_F(StateTest, STATE_SENDING_TimeoutLessThanExpected)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    auto arg_1_1 = 0;
    TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1);
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);


    // [Step 2]: STATE_SENDING: Timeout less than expected. ----------
    ASSERT_EQ(STATE_GET(), STATE_SENDING);

    auto arg_2_1 = 0.2;
    TRANSITION_RUN(STATE_SENDING, STATE_RECEIVING, &arg_2_1);
    HOOK_RUN(update);

    ASSERT_NE(STATE_GET(), STATE_RECEIVING);

}

TEST_F(StateTest, STATE_SENDING_TimeoutGreaterThanExpected)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    auto arg_1_1 = 0;
    TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1);
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);


    // [Step 2]: STATE_SENDING: Timeout greater than expected. ----------
    ASSERT_EQ(STATE_GET(), STATE_SENDING);

    auto arg_2_1 = 3.8;
    TRANSITION_RUN(STATE_SENDING, STATE_RECEIVING, &arg_2_1);
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

}

