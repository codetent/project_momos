#include "hal_mock.hpp"

extern "C"
{
#include "hal.h"
}

extern hal_mock *hal_mock_obj;

void transmit(int a)
{
    hal_mock_obj->transmit(a);
}

bool receive(int *a)
{
    return hal_mock_obj->receive(a);
}
