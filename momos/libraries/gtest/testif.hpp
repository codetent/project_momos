#ifndef _MOMOS_TESTIF_
#define _MOMOS_TESTIF_

#include <chrono>
#include <list>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

template <typename D>
class SimulatedMessage
{
public:
    D *data;
    uint32_t length;
    uint64_t timestamp;

    SimulatedMessage(D *buffer, uint32_t length, uint64_t timestamp)
    {
        this->length = length;
        this->timestamp = timestamp;

        this->data = (D *)malloc(sizeof(D) * length);
        memcpy(this->data, buffer, sizeof(D) * length);
    }

    SimulatedMessage(SimulatedMessage<D> const &msg) : SimulatedMessage(msg.data, msg.length, msg.timestamp) {}

    ~SimulatedMessage()
    {
        free(this->data);
    }
};

template <typename D>
class SimulatedChannel
{
public:
    list<SimulatedMessage<D>> messages;

    uint32_t available()
    {
        return this->messages.size();
    }

    SimulatedMessage<D> get_last_message()
    {
        SimulatedMessage<D> msg = this->messages.back();
        this->messages.pop_back();
        return msg;
    }

    void clear()
    {
        this->messages.clear();
    }
};

template <typename D>
class TestInterface
{
private:
    uint64_t get_current_time()
    {
        chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();
        return (uint64_t)current.time_since_epoch().count();
    }

public:
    SimulatedChannel<D> tx_channel;
    SimulatedChannel<D> rx_channel;

    static TestInterface<D> &get_instance()
    {
        static TestInterface<D> instance;
        return instance;
    }

    void reset()
    {
        this->tx_channel.clear();
        this->rx_channel.clear();
    }

    void transmit(D *buffer, uint32_t length, uint64_t *timestamp)
    {
        uint64_t current_timestamp = get_current_time();

        this->tx_channel.messages.push_back(SimulatedMessage<D>(buffer, length, current_timestamp));

        if (timestamp != NULL)
        {
            *timestamp = current_timestamp;
        }
    }

    bool receive(D *buffer, uint32_t *length, uint64_t *timestamp)
    {
        if (this->rx_channel.messages.empty())
        {
            return false;
        }

        SimulatedMessage<D> msg = this->rx_channel.messages.front();

        memcpy(buffer, msg.data, sizeof(D) * msg.length);

        if (length != NULL)
        {
            *length = msg.length;
        }

        if (timestamp != NULL)
        {
            *timestamp = msg.timestamp;
        }

        this->rx_channel.messages.pop_front();
        return true;
    }
};

#endif // _MOMOS_TESTIF_
