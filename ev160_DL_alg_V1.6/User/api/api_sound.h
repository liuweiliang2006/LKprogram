#include "stm32f10x.h"

#ifndef API_SOUND_H
#define API_SOUND_H

void play_sound(u8 num);
void sound_vol_set(u8 num);
void sound_vol_up(void);
void sound_vol_down(void);

#ifdef DEBUGTEST
void sound_test(void);
#endif

#endif


