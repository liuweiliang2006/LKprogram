#include "stm32f10x.h"

#ifndef API_DELAY_TIMER_H
#define API_DELAY_TIMER_H

void irq_delay_timer(void);
u8 delay_timer_st(u8 num);
void delay_timer_end(u8 num);
u32 get_delay_timer(u8 num);

#endif
