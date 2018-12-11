/******************************************************************************
* �ļ���         : api_delay_timer.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : *
* ����           : �ӿڳ���ʵ����ʱ��ʱ������
*
*******************************************************************************/
#include "hw_lib.h"

#define DT_NUM						2
#define DT_TIME_MAX				600000//10����
 bool DT_en[DT_NUM] = {FALSE, FALSE};
 u32 DT_counter[DT_NUM] = {0};
 bool is_irq_in = FALSE;//��ֹ�����ͻ

void irq_delay_timer(void)
{
	u8 i;
	is_irq_in = TRUE;//��ֹ�����ͻ
	for(i = 0;i < DT_NUM;i++)
	{
		if(DT_en[i])
		{
			DT_counter[i]++;
			if(DT_counter[i] > DT_TIME_MAX)
			{
				DT_counter[i] = 0;
				DT_en[i] = FALSE;
			}
		}
	}
	is_irq_in = FALSE;//��ֹ�����ͻ
}

u8 delay_timer_st(u8 num)
{
	if(DT_en[num])
	{
		return 1;
	}
	else
	{
		DT_counter[num] = 0;
		DT_en[num] = TRUE;
		
		return 0;
	}
}

void delay_timer_end(u8 num)
{
	while(is_irq_in);//��ֹ�����ͻ
	delay_us(2);	
	while(is_irq_in);//��ֹ�����ͻ
	
	DT_en[num] = FALSE;	
	DT_counter[num] = 0;
}

u32 get_delay_timer(u8 num)
{
	return DT_counter[num];
}





/*******************************************END OF FILE******************************************/

