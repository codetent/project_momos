#ifndef __HAL_MOCK__
#define __HAL_MOCK__

#include <gmock/gmock.h>

class Ihal
{
public:
    virtual ~Ihal() {}
    virtual void transmit(int a) = 0;
    virtual bool receive(int *a) = 0;
};

class hal_mock : public Ihal
{
public:
    virtual ~hal_mock() {}
    MOCK_METHOD(void, transmit, (int));
    MOCK_METHOD(bool, receive, (int *));
};

#endif // __HAL_MOCK__
