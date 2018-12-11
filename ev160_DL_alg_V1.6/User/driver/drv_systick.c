/******************************************************************************
* 文件名         : drv_systick.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，systick驱动
*
*******************************************************************************/

#include "hw_lib.h"

volatile static u32 systick_counter = 0;
#define SYSTICK_RELOAD_VALUE_MS				(SystemCoreClock / 1000 / 8)
//#define SYSTICK_RELOAD_VALUE_US				(SystemCoreClock / 1000000 / 9)
void systick_init(void)
{
	/* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(SYSTICK_RELOAD_VALUE_MS)) //1ms
  { 
    /* Capture error */ 
    while (1);
  }
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
void systick_irq_process(void)
{
	systick_counter++;
//	led_reverse(4);
}

//void delay_ms(u32 count)
//{
//	u32 time = 0;
//#ifdef IWDG_FUN
//dog_eat();
//#endif
//	time = systick_counter + count;
//	while(systick_counter != time);
//#ifdef IWDG_FUN
//dog_eat();
//#endif
//}
//void delay_us_timer(u32 nus)
//{
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD; //LOAD 的值
//	ticks=nus*9; //需要的节拍数
//	told=SysTick->VAL; //刚进入时的计数器值
//	while(1)
//	{
//		tnow=SysTick->VAL;
//		if(tnow!=told)
//		{
//			if(tnow<told)tcnt+=told-tnow; //注意 SYSTICK 是一个递减的计数器.
//			else tcnt+=reload-tnow+told;
//			told=tnow;
//			if(tcnt>=ticks)break; //时间超过/等于要延迟的时间,则退出.
//		}
//	};
//	register u32 time, systick_value2, systick_value1;
//	systick_value1 = SysTick->VAL + 9;
//	time = count*9;
//	if(time <= systick_value1)
//	{
//		time = systick_value1 - time;
//		systick_value2 = SysTick->VAL;
//		while((systick_value2 > time) && (systick_value2 < systick_value1)) systick_value2 = SysTick->VAL;  
//	}
//	else
//	{
//		time = 9000 + systick_value1 - time;
//		systick_value2 = SysTick->VAL;
//		while((systick_value2 > time) || (systick_value2 < systick_value1)) systick_value2 = SysTick->VAL;  
//	}
//}


/*******************************************END OF FILE******************************************/

