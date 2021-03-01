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

TEST_F(StateTest, STATE_WAIT__STATE_SEND__TimeoutEqualsExpectedValue)
{

    // [Step 1]: STATE_WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);

}

TEST_F(StateTest, STATE_WAIT__STATE_SEND__TimeoutLessThanExpected)
{

    // [Step 1]: STATE_WAIT: Timeout less than expected. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 0.2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_NE(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);

}

TEST_F(StateTest, STATE_WAIT__STATE_SEND__TimeoutGreaterThanExpected)
{

    // [Step 1]: STATE_WAIT: Timeout greater than expected. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 3.8;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);

}

TEST_F(StateTest, STATE_SEND__STATE_SEND_TIMESTAMP__ExpectedIsSent)
{

    // [Step 1]: STATE_WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);


    // [Step 2]: STATE_SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND);

    double arg_2_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND, STATE_SEND_TIMESTAMP, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SEND -> STATE_SEND_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    CHECK_RUN(STATE_SEND, STATE_SEND_TIMESTAMP);

}

TEST_F(StateTest, STATE_SEND_TIMESTAMP__STATE_RECEIVE__JustExecuteTransition)
{

    // [Step 1]: STATE_WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);


    // [Step 2]: STATE_SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND);

    double arg_2_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND, STATE_SEND_TIMESTAMP, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SEND -> STATE_SEND_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    CHECK_RUN(STATE_SEND, STATE_SEND_TIMESTAMP);


    // [Step 3]: STATE_SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    double arg_3_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE, &arg_3_1))
    {
        WARN("Transition undefined for STATE_SEND_TIMESTAMP -> STATE_RECEIVE");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    CHECK_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE);

}

TEST_F(StateTest, STATE_RECEIVE__STATE_RECEIVE_TIMESTAMP__ExpectedIsReceived)
{

    // [Step 1]: STATE_WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);


    // [Step 2]: STATE_SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND);

    double arg_2_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND, STATE_SEND_TIMESTAMP, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SEND -> STATE_SEND_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    CHECK_RUN(STATE_SEND, STATE_SEND_TIMESTAMP);


    // [Step 3]: STATE_SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    double arg_3_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE, &arg_3_1))
    {
        WARN("Transition undefined for STATE_SEND_TIMESTAMP -> STATE_RECEIVE");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    CHECK_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE);


    // [Step 4]: STATE_RECEIVE: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    double arg_4_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP, &arg_4_1))
    {
        WARN("Transition undefined for STATE_RECEIVE -> STATE_RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVE_TIMESTAMP);

    CHECK_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP);

}

TEST_F(StateTest, STATE_RECEIVE__STATE_RECEIVE_TIMESTAMP__NoMessageIsReceived)
{

    // [Step 1]: STATE_WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);


    // [Step 2]: STATE_SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND);

    double arg_2_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND, STATE_SEND_TIMESTAMP, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SEND -> STATE_SEND_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    CHECK_RUN(STATE_SEND, STATE_SEND_TIMESTAMP);


    // [Step 3]: STATE_SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    double arg_3_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE, &arg_3_1))
    {
        WARN("Transition undefined for STATE_SEND_TIMESTAMP -> STATE_RECEIVE");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    CHECK_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE);


    // [Step 4]: STATE_RECEIVE: No message is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);


    ASSERT_NE(STATE_GET(), STATE_RECEIVE_TIMESTAMP);

    CHECK_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP);

}

TEST_F(StateTest, STATE_RECEIVE__STATE_RECEIVE_TIMESTAMP__MoreMessagesAreReceivedThanExpected)
{

    // [Step 1]: STATE_WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);


    // [Step 2]: STATE_SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND);

    double arg_2_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND, STATE_SEND_TIMESTAMP, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SEND -> STATE_SEND_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    CHECK_RUN(STATE_SEND, STATE_SEND_TIMESTAMP);


    // [Step 3]: STATE_SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    double arg_3_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE, &arg_3_1))
    {
        WARN("Transition undefined for STATE_SEND_TIMESTAMP -> STATE_RECEIVE");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    CHECK_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE);


    // [Step 4]: STATE_RECEIVE: More messages are received than expected. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    double arg_4_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP, &arg_4_1))
    {
        WARN("Transition undefined for STATE_RECEIVE -> STATE_RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(update);
    double arg_4_2 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP, &arg_4_2))
    {
        WARN("Transition undefined for STATE_RECEIVE -> STATE_RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_NE(STATE_GET(), STATE_RECEIVE_TIMESTAMP);

    CHECK_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP);

}

TEST_F(StateTest, STATE_RECEIVE_TIMESTAMP__STATE_WAIT__JustExecuteTransition)
{

    // [Step 1]: STATE_WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    double arg_1_1 = 2;
    if (!TRANSITION_RUN(STATE_WAIT, STATE_SEND, &arg_1_1))
    {
        WARN("Transition undefined for STATE_WAIT -> STATE_SEND");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND);

    CHECK_RUN(STATE_WAIT, STATE_SEND);


    // [Step 2]: STATE_SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND);

    double arg_2_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND, STATE_SEND_TIMESTAMP, &arg_2_1))
    {
        WARN("Transition undefined for STATE_SEND -> STATE_SEND_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    CHECK_RUN(STATE_SEND, STATE_SEND_TIMESTAMP);


    // [Step 3]: STATE_SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_GET(), STATE_SEND_TIMESTAMP);

    double arg_3_1 = 0;
    if (!TRANSITION_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE, &arg_3_1))
    {
        WARN("Transition undefined for STATE_SEND_TIMESTAMP -> STATE_RECEIVE");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    CHECK_RUN(STATE_SEND_TIMESTAMP, STATE_RECEIVE);


    // [Step 4]: STATE_RECEIVE: Expected is received. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVE);

    double arg_4_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP, &arg_4_1))
    {
        WARN("Transition undefined for STATE_RECEIVE -> STATE_RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_RECEIVE_TIMESTAMP);

    CHECK_RUN(STATE_RECEIVE, STATE_RECEIVE_TIMESTAMP);


    // [Step 5]: STATE_RECEIVE_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_GET(), STATE_RECEIVE_TIMESTAMP);

    double arg_5_1 = 0;
    if (!TRANSITION_RUN(STATE_RECEIVE_TIMESTAMP, STATE_WAIT, &arg_5_1))
    {
        WARN("Transition undefined for STATE_RECEIVE_TIMESTAMP -> STATE_WAIT");
    }
    HOOK_RUN(update);

    ASSERT_EQ(STATE_GET(), STATE_WAIT);

    CHECK_RUN(STATE_RECEIVE_TIMESTAMP, STATE_WAIT);

}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}