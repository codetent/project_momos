#ifndef _HAL_H
#define _HAL_H

#include <stdbool.h>
#include <stdint.h>

void transmit(int a);
bool receive(int *a);
uint32_t current_time(void);

#endif // _HAL_H
