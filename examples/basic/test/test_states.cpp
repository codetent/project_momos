#include <iostream>
#include <gtest/gtest.h>

#include "momos/io.hpp"
#include "momos/macros.hpp"

#include "base_states.cpp"


#define WARN(x) std::cerr << "[     WARN ] " << x << std::endl


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

TEST_F(StateTest, STATE_RECEIVING__STATE_SENDING__ExpectedIsReceived)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    double arg_1_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1))
    {
        WARN("Transition undefined for STATE_RECEIVING -> STATE_SENDING");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);

}

TEST_F(StateTest, STATE_RECEIVING__STATE_SENDING__NoMessageIsReceived)
{

    // [Step 1]: STATE_RECEIVING: No message is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);


    ASSERT_NE(STATE_GET(), STATE_SENDING);

}

TEST_F(StateTest, STATE_SENDING__STATE_RECEIVING__TimeoutEqualsExpectedValue)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    double arg_1_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1))
    {
        WARN("Transition undefined for STATE_RECEIVING -> STATE_SENDING");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);


    // [Step 2]: STATE_SENDING: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_SENDING);

    double arg_2_1 = 2;
    if (!TRANSITION_RUN(STATE_SENDING, STATE_RECEIVING, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SENDING -> STATE_RECEIVING");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

}

TEST_F(StateTest, STATE_SENDING__STATE_RECEIVING__TimeoutLessThanExpected)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    double arg_1_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1))
    {
        WARN("Transition undefined for STATE_RECEIVING -> STATE_SENDING");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);


    // [Step 2]: STATE_SENDING: Timeout less than expected. ----------
    ASSERT_EQ(STATE_GET(), STATE_SENDING);

    double arg_2_1 = 0.2;
    if (!TRANSITION_RUN(STATE_SENDING, STATE_RECEIVING, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SENDING -> STATE_RECEIVING");
    }
    HOOK_RUN(update);

    ASSERT_NE(STATE_GET(), STATE_RECEIVING);

}

TEST_F(StateTest, STATE_SENDING__STATE_RECEIVING__TimeoutGreaterThanExpected)
{

    // [Step 1]: STATE_RECEIVING: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

    double arg_1_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVING, STATE_SENDING, &arg_1_1))
    {
        WARN("Transition undefined for STATE_RECEIVING -> STATE_SENDING");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SENDING);


    // [Step 2]: STATE_SENDING: Timeout greater than expected. ----------
    ASSERT_EQ(STATE_GET(), STATE_SENDING);

    double arg_2_1 = 3.8;
    if (!TRANSITION_RUN(STATE_SENDING, STATE_RECEIVING, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SENDING -> STATE_RECEIVING");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVING);

}

