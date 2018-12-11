

#include "stm32f10x.h"

#ifndef API_ADC_H
#define API_ADC_H

typedef enum {NOTRIGGER = 0, PRETRIGGER = 1,TRIGGERED = 2} TriggerStatus;//未触发，预触发，已触发完成

//#define DEBUGTEST
//#define AUTOSAMPLING
#define MAINRUN

//#define IWDG_FUN

#define CHANNEL    					12
#define FILTER_CNT          512//256
#define BackSampl_CNT				256
#define AD_TEMP_CNT					50//32	
#define MULTIPLE 					  2
#define TOLERANCE 					15

extern bool Admit_SampleAD;
extern TriggerStatus trigger_status;	
extern vu16 lvl_orig[2*CHANNEL];
extern u16 Threshold_Max[CHANNEL];
extern u16 Threshold_Min[CHANNEL];//AD触发门限
extern u16 AD_store_final[CHANNEL][FILTER_CNT];//最终给用户的AD数据	
extern u16 AD_value[CHANNEL];

void dma1_irq_adc_process(void);
void adc_adjust(u32 adjust);
void adc_set_threshold(u16 tol);

#endif








