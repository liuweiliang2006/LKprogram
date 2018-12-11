/******************************************************************************
* �ļ���         : drv_gpio_int.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : 2018.07
* ����           : ��������gpio�ж���������
*
*******************************************************************************/

#include "hw_lib.h"	



void gpio_int_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);
		
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //���� EXTI_InitStruct ��ָ����
	
}

void gpio_int_process(void)
{
	led_twinkle(4, 2, 500);
}

/*******************************************END OF FILE******************************************/



