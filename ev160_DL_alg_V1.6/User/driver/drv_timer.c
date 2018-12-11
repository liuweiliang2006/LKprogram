/******************************************************************************
* 文件名         : drv_timer.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，定时器驱动
*
*******************************************************************************/

#include "hw_lib.h"

#ifdef	TIMER_FUN

static void TLT_irq_process(void);
volatile static u32 Timer_Counter = 0;
volatile static u32 Timer_ms = 0;
volatile static u32 Timer_s = 0;
void timer_init(void)
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = 999;//100us * 10 = 1ms
	TIM_TimeBaseStructure.TIM_Prescaler =((SystemCoreClock/1000000) - 1);//1us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );
	
	TIM_Cmd(TIM2, ENABLE);
}
void timer_irq_process(void)
{
	Timer_Counter++;
	TLT_irq_process();
	shake_time_process();
	Timer_ms++;
	if(Timer_ms==1000)
	{
		Timer_ms=0;
		Timer_s++;	
	}
//	led_reverse(4);
	irq_delay_timer();
}
u32 get_timer_counter(void)
{
	return Timer_Counter;
}
u32 get_timer_s(void)
{
	return Timer_s;
}
void delay_ms(u32 count)
{
	u32 time = 0;
#ifdef IWDG_FUN
dog_eat();
#endif
	time = Timer_Counter + count;
	while(Timer_Counter != time)
	{
#ifdef IWDG_FUN
dog_eat();
#endif
	}
}
volatile static bool TLT_EN = FALSE; 
volatile static u16 TLT_counter = 0;
volatile static u16 TLT_time = 0;
volatile static u8 TLT_led_num = 0;
volatile static u16 TLT_twinkle_times = 0;
u8 timer_led_twinkle(u8 num, u16 twinkle_times, u16 times)
{
	u16 i;
	if(TLT_EN)
	{
		return 1;
	}
	else
	{
		for(i = 0;i < 4;i++) led_off(i);
		TLT_counter = 0;
		TLT_twinkle_times = twinkle_times << 1;
		TLT_time = times;
		TLT_led_num = num;
		
		TLT_EN = TRUE;
		return 0;
	}	
}
void timer_led_twinkle_off(void)
{
	TLT_EN = FALSE;
}
bool TLT_led_reverse_STATE = TRUE;
static void TLT_led_reverse(void)
{
	u16 i;
	if(TLT_led_num != 4)
	{
		if(TLT_led_reverse_STATE)
		{
			led_on(TLT_led_num);
			TLT_led_reverse_STATE = FALSE;
		}
		else
		{
			led_off(TLT_led_num);
			TLT_led_reverse_STATE = TRUE;
		}
	}
	else
	{
		if(TLT_led_reverse_STATE)
		{
			for(i = 0;i < 4;i++) led_on(i);
			TLT_led_reverse_STATE = FALSE;
		}
		else
		{
			for(i = 0;i < 4;i++) led_off(i);
			TLT_led_reverse_STATE = TRUE;
		}
	}
}
static void TLT_irq_process(void)
{
	if(TLT_EN)
	{
		TLT_counter++;
		if(TLT_counter > TLT_time)
		{
			TLT_counter = 0;
			TLT_led_reverse();
			TLT_twinkle_times--;
		}
		if(TLT_twinkle_times == 0)
		{
			TLT_EN = FALSE;
		}
	}
}





#endif

/*******************************************END OF FILE******************************************/


