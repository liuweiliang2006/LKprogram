#include "stm32f10x.h"

#ifndef DRV_API_BATTERY_H
#define DRV_API_BATTERY_H

#ifdef BATTERY_MONITOR

#define REFER_VDDA			330
#define MAX_ADC					4096

#define POWER80					390
#define POWER50					370
#define POWER15					360
#define POWER5					350

void led_reverse(u8 num);
void led_twinkle(u8 num, u8 twinkle_times, u16 times);

void display_battery(void);
void display_battery_first(void);
void gener_shake_signal(u16 time);

#endif

#endif


