#include "../momos/testif.hpp"
#include <stdio.h>

extern "C"
{
#include "hal.h"
}

void transmit(int a)
{
    TestInterface<int> &interface = TestInterface<int>::get_instance();
    interface.transmit(&a, 1U, NULL);
}

bool receive(int *a)
{
    TestInterface<int> &interface = TestInterface<int>::get_instance();
    return interface.receive(a, NULL, NULL);
}
