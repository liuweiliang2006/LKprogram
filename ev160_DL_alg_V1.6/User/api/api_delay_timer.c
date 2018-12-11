/******************************************************************************
* 文件名         : api_delay_timer.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 接口程序，实现延时定时器功能
*
*******************************************************************************/
#include "hw_lib.h"

#define DT_NUM						2
#define DT_TIME_MAX				600000//10分钟
 bool DT_en[DT_NUM] = {FALSE, FALSE};
 u32 DT_counter[DT_NUM] = {0};
 bool is_irq_in = FALSE;//防止句柄冲突

void irq_delay_timer(void)
{
	u8 i;
	is_irq_in = TRUE;//防止句柄冲突
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
	is_irq_in = FALSE;//防止句柄冲突
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
	while(is_irq_in);//防止句柄冲突
	delay_us(2);	
	while(is_irq_in);//防止句柄冲突
	
	DT_en[num] = FALSE;	
	DT_counter[num] = 0;
}

u32 get_delay_timer(u8 num)
{
	return DT_counter[num];
}





/*******************************************END OF FILE******************************************/

