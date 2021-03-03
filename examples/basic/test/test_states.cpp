#include <iostream>

#include <gtest/gtest.h>
#include <momos.hpp>

#include "base_states.cpp"


#define WARN(x) std::cerr << "[     WARN ] " << x << std::endl


class StateEnv : public ::testing::Environment
{
public:
    void SetUp() override {
        HOOK_RUN(before_all);
    }

    void TearDown() override {
        HOOK_RUN(after_all);
    }
};


class StateTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        HOOK_RUN(before);
    }

    void TearDown() override
    {
        HOOK_RUN(after);
    }
};

TEST_F(StateTest, WAIT__SEND__TimeoutEqualsExpectedValue)
{

    // [Step 1]: WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

}

TEST_F(StateTest, WAIT__SEND__TimeoutLessThanExpected)
{

    // [Step 1]: WAIT: Timeout less than expected. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 0.1;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_NE(STATE_VAR_GET(), STATE_GET(SEND));

}

TEST_F(StateTest, WAIT__SEND__TimeoutGreaterThanExpected)
{

    // [Step 1]: WAIT: Timeout greater than expected. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.9;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

}

TEST_F(StateTest, SEND__SEND_TIMESTAMP__ExpectedIsSent)
{

    // [Step 1]: WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, correct, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

}

TEST_F(StateTest, SEND_TIMESTAMP__RECEIVE__JustExecuteTransition)
{

    // [Step 1]: WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, correct, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, None, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

}

TEST_F(StateTest, RECEIVE__RECEIVE_TIMESTAMP__ExpectedIsReceived)
{

    // [Step 1]: WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, correct, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, None, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: Expected is received. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 0;
    if (!PREPARE_RUN(RECEIVE, RECEIVE_TIMESTAMP, receive, None, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE_TIMESTAMP));

}

TEST_F(StateTest, RECEIVE__RECEIVE_TIMESTAMP__NoMessageIsReceived)
{

    // [Step 1]: WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, correct, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, None, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: No message is received. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    ASSERT_NE(STATE_VAR_GET(), STATE_GET(RECEIVE_TIMESTAMP));

}

TEST_F(StateTest, RECEIVE__RECEIVE_TIMESTAMP__MoreMessagesAreReceivedThanExpected)
{

    // [Step 1]: WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, correct, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, None, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: More messages are received than expected. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 0;
    if (!PREPARE_RUN(RECEIVE, RECEIVE_TIMESTAMP, receive, None, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(progress);
    auto arg_4_2 = 0;
    if (!PREPARE_RUN(RECEIVE, RECEIVE_TIMESTAMP, receive, None, &arg_4_2))
    {
        WARN("Transition preparation undefined for RECEIVE -> RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_NE(STATE_VAR_GET(), STATE_GET(RECEIVE_TIMESTAMP));

}

TEST_F(StateTest, RECEIVE_TIMESTAMP__WAIT__JustExecuteTransition)
{

    // [Step 1]: WAIT: Timeout equals expected value. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Expected is sent. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, correct, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, None, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: Expected is received. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 0;
    if (!PREPARE_RUN(RECEIVE, RECEIVE_TIMESTAMP, receive, None, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE_TIMESTAMP));


    // [Step 5]: RECEIVE_TIMESTAMP: Just execute transition. ----------
    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE_TIMESTAMP));

    auto arg_5_1 = 0;
    if (!PREPARE_RUN(RECEIVE_TIMESTAMP, WAIT, None, None, &arg_5_1))
    {
        WARN("Transition preparation undefined for RECEIVE_TIMESTAMP -> WAIT");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new StateEnv());
    return RUN_ALL_TESTS();
}