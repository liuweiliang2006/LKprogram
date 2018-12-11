/******************************************************************************
* �ļ���         : drv_adc.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : 2018.07
* ����           : ��������ADC����
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hw_lib.h"	

/*******************************************************************************
* ��������       : ADC1_Init
* ����           : ��ʼ��ADC1
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
void ADC1_Init(void)
{	
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��
	
	RCC_ADCCLKConfig(ADC_RCC_DIV);   //����ADC��Ƶ����8 48M/8=6,ADC���ʱ�䲻�ܳ���14M
  ADC_DeInit(ADC1);  
  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ʹ�ö���ģʽ��ɨ��ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//������Ӵ�����
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ʹ�������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = (CHANNEL);  // 4��ת��ͨ��  CHANNEL
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel configuration */ 
#ifdef CHANNEL_4
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH0, 1, ADC_SAMPLE_TIMES); 
  ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH1, 2, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH2, 3, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH3, 4, ADC_SAMPLE_TIMES);	
#endif

#ifdef CHANNEL_8
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH0, 1, ADC_SAMPLE_TIMES); 
  ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH1, 2, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH2, 3, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH3, 4, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH4, 5, ADC_SAMPLE_TIMES); 
  ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH5, 6, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH6, 7, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH7, 8, ADC_SAMPLE_TIMES);	
#endif
	
#ifdef CHANNEL_12
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH0, 1, ADC_SAMPLE_TIMES); 
  ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH1, 2, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH2, 3, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH3, 4, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH4, 5, ADC_SAMPLE_TIMES); 
  ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH5, 6, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH6, 7, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH7, 8, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH8, 9, ADC_SAMPLE_TIMES); 
  ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH9, 10, ADC_SAMPLE_TIMES); 
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH10, 11, ADC_SAMPLE_TIMES);
	ADC_RegularChannelConfig(ADC1, ADC_CONFIG_CH11, 12, ADC_SAMPLE_TIMES);	
#endif

#ifdef BATTERY_MONITOR
	ADC_InjectedSequencerLengthConfig(ADC1, 1);
	ADC_InjectedChannelConfig(ADC1, ADC_CONFIG_BATTERY_CH, 1, ADC_BATTERY_SAMPLE_TIMES);
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);
	ADC_ExternalTrigInjectedConvCmd(ADC1, DISABLE);
	ADC_AutoInjectedConvCmd(ADC1, DISABLE);

#endif
	
	
	/* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA
	
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));   

	/* Enable DMA channel1 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��MDA1ʱ��
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//����DMA�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = DMA_DSTADR;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_BufferSize =DMA_BUFFERSIZE;	//DMA����������Ϊ1��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);	

//	DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC , ENABLE);	
		
	/* Start ADC1 Software Conversion */ 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //��ʼת��
	ADC_start();
} 

void ADC_start(void)
{
	DMA_Cmd(DMA1_Channel1, ENABLE); //
	delay_ms(100);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void ADC_stop(void)
{
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE); //ֹͣDMAͨ��
}

#ifdef BATTERY_MONITOR
u16 get_injected_value(void)
{
	u16 value;
	ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
	delay_ms(1);
	value = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);	
	return value;
}
#endif

#ifdef DEBUGTEST

u16 temp_injected_buf[256] = {0};
u16 temp_injected_i = 0;
void test_injected(void)
{
	temp_injected_buf[temp_injected_i++] = get_injected_value();
	if(temp_injected_i == 256) 
		temp_injected_i = 0;
}

#endif

/*******************************************END OF FILE******************************************/


