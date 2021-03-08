#include <iostream>

#include <gtest/gtest.h>
#include <momos.hpp>

#include "base_states.cpp"


#define LOG(x)  std::cout << "[ #        ] " << x << std::endl 
#define WARN(x) std::cerr << "[ !        ] " << x << std::endl


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

TEST_F(StateTest, WAIT__SEND__timeout__ok)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

}

TEST_F(StateTest, WAIT__SEND__timeout__earlier)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout less than expected. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 0.1;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_NE(STATE_VAR_GET(), STATE_GET(SEND));

}

TEST_F(StateTest, WAIT__SEND__timeout__later)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout greater than expected. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.9;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

}

TEST_F(StateTest, SEND__SEND_TIMESTAMP__transmit__ok)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

}

TEST_F(StateTest, RECEIVE__RECEIVE_TIMESTAMP__receive__correct__ok)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Timeout matches expected value. ----------
    LOG("SEND_TIMESTAMP -> RECEIVE [default]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, default, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: Message is received as expected. ----------
    LOG("RECEIVE -> RECEIVE_TIMESTAMP [receive#correct]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 0;
    if (!PREPARE_RUN(RECEIVE, RECEIVE_TIMESTAMP, receive, correct, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> RECEIVE_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE_TIMESTAMP));

}

TEST_F(StateTest, RECEIVE__WAIT__receive__incorrect__no)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Timeout matches expected value. ----------
    LOG("SEND_TIMESTAMP -> RECEIVE [default]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, default, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: No message is received. ----------
    LOG("RECEIVE -> WAIT [receive#incorrect]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    ASSERT_NE(STATE_VAR_GET(), STATE_GET(WAIT));

}

TEST_F(StateTest, RECEIVE__WAIT__timeout__ok)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Timeout matches expected value. ----------
    LOG("SEND_TIMESTAMP -> RECEIVE [default]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, default, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: Timeout matches expected value. ----------
    LOG("RECEIVE -> WAIT [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 1.0;
    if (!PREPARE_RUN(RECEIVE, WAIT, timeout, None, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> WAIT");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

}

TEST_F(StateTest, RECEIVE__WAIT__timeout__earlier)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Timeout matches expected value. ----------
    LOG("SEND_TIMESTAMP -> RECEIVE [default]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, default, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: Timeout less than expected. ----------
    LOG("RECEIVE -> WAIT [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 0.1;
    if (!PREPARE_RUN(RECEIVE, WAIT, timeout, None, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> WAIT");
    }
    HOOK_RUN(progress);

    ASSERT_NE(STATE_VAR_GET(), STATE_GET(WAIT));

}

TEST_F(StateTest, RECEIVE__RECEIVE_TIMESTAMP__receive__correct__no)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Timeout matches expected value. ----------
    LOG("SEND_TIMESTAMP -> RECEIVE [default]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, default, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: No message is received. ----------
    LOG("RECEIVE -> RECEIVE_TIMESTAMP [receive#correct]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    ASSERT_NE(STATE_VAR_GET(), STATE_GET(RECEIVE_TIMESTAMP));

}

TEST_F(StateTest, RECEIVE__WAIT__receive__incorrect__ok)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Timeout matches expected value. ----------
    LOG("SEND_TIMESTAMP -> RECEIVE [default]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, default, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: Message is received as expected. ----------
    LOG("RECEIVE -> WAIT [receive#incorrect]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 0;
    if (!PREPARE_RUN(RECEIVE, WAIT, receive, incorrect, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> WAIT");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

}

TEST_F(StateTest, RECEIVE__WAIT__timeout__later)
{
    LOG("Description: ");


    // [Step 1]: WAIT: Timeout matches expected value. ----------
    LOG("WAIT -> SEND [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(WAIT));

    auto arg_1_1 = 1.0;
    if (!PREPARE_RUN(WAIT, SEND, timeout, None, &arg_1_1))
    {
        WARN("Transition preparation undefined for WAIT -> SEND");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));


    // [Step 2]: SEND: Message is sent as expected. ----------
    LOG("SEND -> SEND_TIMESTAMP [transmit]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND));

    auto arg_2_1 = 0;
    if (!PREPARE_RUN(SEND, SEND_TIMESTAMP, transmit, None, &arg_2_1))
    {
        WARN("Transition preparation undefined for SEND -> SEND_TIMESTAMP");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));


    // [Step 3]: SEND_TIMESTAMP: Timeout matches expected value. ----------
    LOG("SEND_TIMESTAMP -> RECEIVE [default]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(SEND_TIMESTAMP));

    auto arg_3_1 = 0;
    if (!PREPARE_RUN(SEND_TIMESTAMP, RECEIVE, default, None, &arg_3_1))
    {
        WARN("Transition preparation undefined for SEND_TIMESTAMP -> RECEIVE");
    }
    HOOK_RUN(progress);

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));


    // [Step 4]: RECEIVE: Timeout greater than expected. ----------
    LOG("RECEIVE -> WAIT [timeout]");

    ASSERT_EQ(STATE_VAR_GET(), STATE_GET(RECEIVE));

    auto arg_4_1 = 1.9;
    if (!PREPARE_RUN(RECEIVE, WAIT, timeout, None, &arg_4_1))
    {
        WARN("Transition preparation undefined for RECEIVE -> WAIT");
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