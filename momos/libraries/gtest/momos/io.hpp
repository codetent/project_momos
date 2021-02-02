#ifndef __MOMOS_IO__
#define __MOMOS_IO__

#include <stdint.h>
#include <list>
#include <memory>
#include <cstring>

struct Message
{
    void *buffer;
    uint32_t length;
    uint64_t timestamp;
};

class Channel
{
private:
    std::list<Message> messages;

public:
    bool available(void)
    {
        return this->messages.size() > 0U;
    }

    void provide(void *buffer, uint32_t length, uint64_t timestamp)
    {
        void *msg_buffer = malloc(length);

        memcpy(msg_buffer, buffer, length);
        this->messages.push_back({
            .buffer = msg_buffer,
            .length = length,
            .timestamp = timestamp,
        });
    }

    bool request(void *buffer, uint32_t *length, uint64_t *timestamp)
    {
        if (this->messages.size() == 0U)
        {
            return false;
        }

        Message msg = this->messages.front();

        memcpy(buffer, msg.buffer, msg.length);
        *length = msg.length;

        if (timestamp != NULL)
        {
            *timestamp = msg.timestamp;
        }

        this->messages.pop_front();
        return true;
    }

    void reset(void)
    {
        this->messages.clear();
    }
};

namespace io
{
    extern Channel in;
    extern Channel out;
} // namespace io

#endif // __MOMOS_IO__
