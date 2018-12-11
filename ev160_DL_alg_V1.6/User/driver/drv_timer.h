#include "stm32f10x.h"

#ifndef DRV_TIMER_H
#define DRV_TIMER_H

#define TIMER_FUN

#ifdef	TIMER_FUN

void timer_init(void);
void timer_irq_process(void);
void delay_ms(u32 count);
u32 get_timer_counter(void);
u32 get_timer_s(void);
u8 timer_led_twinkle(u8 num, u16 twinkle_times, u16 times);
void timer_led_twinkle_off(void);
	
#endif

#endif



