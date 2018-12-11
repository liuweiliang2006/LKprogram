

#include "stm32f10x.h"

#ifndef API_SHAKE_H
#define API_SHAKE_H

#define SHAKE_BEGING				(led_off(5))
#define SHAKE_END						(led_on(5))
void shake_time_process(void);
void gener_shake(u32 time);

#endif








