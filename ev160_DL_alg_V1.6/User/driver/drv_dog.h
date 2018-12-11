#include "stm32f10x.h"

#ifndef DRV_DOG_H
#define DRV_DOG_H



#ifdef IWDG_FUN
void dog_init(void);
void dog_en(void);
void dog_eat(void);
#endif

#endif



/*******************************************END OF FILE******************************************/
