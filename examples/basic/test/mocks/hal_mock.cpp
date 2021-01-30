
#include <stdio.h>
#include "momos/io.hpp"

extern "C"
{
#include "hal.h"
}

void transmit(int a)
{
    io::in.provide(&a, (uint32_t)sizeof(int), 0U);
}

bool receive(int *a)
{
    uint32_t length;
    return io::out.request(a, &length, NULL);
}
