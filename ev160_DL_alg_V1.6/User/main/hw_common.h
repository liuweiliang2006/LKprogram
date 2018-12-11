


#include "stm32f10x.h"

#ifndef HW_COMMON_H
#define HW_COMMON_H

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum {CLOSE = 0, OPEN = !CLOSE, UNKNOW=2 } DoorState;

void delay_us(u32 count);
void delay_100us(u32 count);
//void delay_ms(u32 count);

#endif
