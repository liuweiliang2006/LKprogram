#include "stm32f10x.h"



#ifndef DRV_SOUND_H
#define DRV_SOUND_H

void sound_senddata(u8 buf[], u16 len);
u16 sound_getdata(u8 buf[]);


#endif

