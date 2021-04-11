#include "hal.h"

#include <stdio.h>
#include <time.h>

void transmit(int a)
{
    printf("transmit: %d\n", a);
}

bool receive(int *a)
{
    *a = 0;
    return true;
}

uint32_t current_time(void)
{
    return (uint32_t)time(0);
}
