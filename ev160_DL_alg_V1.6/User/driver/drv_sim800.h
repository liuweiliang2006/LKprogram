#include "stm32f10x.h"



#ifndef DRV_SIM800_H
#define DRV_SIM800_H

void sim_rev_irq_process(char rev_data);
void sim_test(void);
void send_sms(u8 num);
bool find_string(char* p);

#endif

