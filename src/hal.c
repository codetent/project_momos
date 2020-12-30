#include "hal.h"

#include <stdio.h>

void transmit(int a)
{
    printf("transmit: %d\n", a);
}

bool receive(int *a)
{
    *a = 0;
    return true;
}
