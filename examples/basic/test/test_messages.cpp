#include <gtest/gtest.h>
#include "momos/testif.hpp"
#include "momos/builder.hpp"

extern "C"
{
#include "messages.h"
#include "logic.h"

#include "../src/states.c"
}

#define INIT() logic_init()
#define UPDATE() logic_run()
#define CURRENT_STATE current_state

TEST(StateTest, example)
{
    INIT();

    ASSERT_EQ(CURRENT_STATE, STATE_RECEIVING);

    MessageBuilder<void, int> builder(send_message);
    SimulatedMessage<int> msg = builder.call<int>(7);

    TestInterface<int> &interface = TestInterface<int>::get_instance();
    interface.rx_channel.messages.push_back(msg);

    UPDATE();

    ASSERT_EQ(CURRENT_STATE, STATE_SENDING);
}
