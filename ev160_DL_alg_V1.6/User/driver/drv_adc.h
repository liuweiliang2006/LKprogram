
#include "stm32f10x.h"

#ifndef DRV_ADC_H
#define DRV_ADC_H

#define ADC_RCC_DIV					RCC_PCLK2_Div6
//#define ADC_SAMPLE_TIMES			ADC_SampleTime_1Cycles5  //12ch,71.429khz
#define ADC_SAMPLE_TIMES			ADC_SampleTime_28Cycles5 //12ch,24.39khz
#define DMA_BUFFERSIZE			(CHANNEL)
#define DMA_DSTADR					((u32)(&AD_value))

//#define CHANNEL_4
//#define CHANNEL_8
#define CHANNEL_12
#define BATTERY_MONITOR

#ifdef CHANNEL_4
#define ADC_CONFIG_CH0			ADC_Channel_6
#define ADC_CONFIG_CH1			ADC_Channel_7
#define ADC_CONFIG_CH2			ADC_Channel_8
#define ADC_CONFIG_CH3			ADC_Channel_9
#endif

#ifdef CHANNEL_8
#define ADC_CONFIG_CH0			ADC_Channel_0
#define ADC_CONFIG_CH1			ADC_Channel_1
#define ADC_CONFIG_CH2			ADC_Channel_2
#define ADC_CONFIG_CH3			ADC_Channel_3
#define ADC_CONFIG_CH4			ADC_Channel_4
#define ADC_CONFIG_CH5			ADC_Channel_5
#define ADC_CONFIG_CH6			ADC_Channel_6
#define ADC_CONFIG_CH7			ADC_Channel_7
#endif

#ifdef CHANNEL_12
#define ADC_CONFIG_CH0			ADC_Channel_0
#define ADC_CONFIG_CH1			ADC_Channel_1
#define ADC_CONFIG_CH2			ADC_Channel_2
#define ADC_CONFIG_CH3			ADC_Channel_3
#define ADC_CONFIG_CH4			ADC_Channel_4
#define ADC_CONFIG_CH5			ADC_Channel_5
#define ADC_CONFIG_CH6			ADC_Channel_6
#define ADC_CONFIG_CH7			ADC_Channel_7
#define ADC_CONFIG_CH8			ADC_Channel_14
#define ADC_CONFIG_CH9			ADC_Channel_15
#define ADC_CONFIG_CH10			ADC_Channel_8
#define ADC_CONFIG_CH11			ADC_Channel_9
#endif

void ADC1_Init(void);
void ADC_start(void);
void ADC_stop(void);

#ifdef BATTERY_MONITOR
#define ADC_CONFIG_BATTERY_CH			ADC_Channel_13
#define ADC_BATTERY_SAMPLE_TIMES	ADC_SampleTime_239Cycles5

u16 get_injected_value(void);
#endif

#ifdef DEBUGTEST
void test_injected(void);
#endif

#endif


