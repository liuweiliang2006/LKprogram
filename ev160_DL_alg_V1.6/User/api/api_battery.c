/******************************************************************************
* 文件名         : api_battery.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 接口程序，电池电量监控显示接口程序
*
*******************************************************************************/

#include "hw_lib.h"	

#ifdef BATTERY_MONITOR

#define LED_INTERVAL_TIME				200

bool LED_REVERSE_STATE = TRUE;
void led_reverse(u8 num)
{
	if(LED_REVERSE_STATE)
	{
		led_on(num);
		LED_REVERSE_STATE = FALSE;
	}
	else
	{
		led_off(num);
		LED_REVERSE_STATE = TRUE;
	}
}

void led_twinkle(u8 num, u8 twinkle_times, u16 times)
{
	u16 i,j;
	
	for(i = 0;i < 4;i++) led_off(i);	
	delay_ms(times);
	delay_ms(times);
	if(num != 4)
	{
		for(j = 0;j < twinkle_times;j++)
		{
			led_on(num);
			delay_ms(times);
			led_off(num);
			delay_ms(times);
		}
		led_on(num);
		delay_ms(times);
	}
	else
	{
		for(j = 0;j < twinkle_times;j++)
		{
			for(i = 0;i < 4;i++) led_on(i);
			delay_ms(times);
			for(i = 0;i < 4;i++) led_off(i);
			delay_ms(times);
		}
		for(i = 0;i < 4;i++) led_on(i);
		delay_ms(times);
	}
//	display_battery();	
}
static u16 get_battery_voltage(void)
{
	u16 v1 = 0, i = 0;
	u32 v2 = 0;
	
	for(i = 0;i < 4;i++)
	{
		v1 += get_injected_value();
	}	
	v1 >>= 2;
	v2 = v1*REFER_VDDA;
	v2 = v2/MAX_ADC;	
	
	
	return (u16)(v2<<1);
}
static void display_battery_led(u16 num, u16 times)
{
	u16 i;
	for(i = 0;i < num;i++)
	{
		led_on(i);
	}
	for(;i < 4;i++)
	{
		led_off(i);
	}
}
void display_battery(void)
{
	u16 v = 0;
	v = get_battery_voltage();	
	if(v > POWER80)      display_battery_led(4, LED_INTERVAL_TIME);
	else if(v > POWER50) display_battery_led(3, LED_INTERVAL_TIME);
	else if(v > POWER15) display_battery_led(2, LED_INTERVAL_TIME);	
	else if(v > POWER5)  
	{
//		play_sound(14);
		display_battery_led(1, LED_INTERVAL_TIME);
	}
	else                 
	{
//		play_sound(15);
		display_battery_led(0, LED_INTERVAL_TIME);	
	}
}
static void display_battery_led_first(u16 num, u16 times)
{
	u16 i;
	for(i = 0;i < 4;i++)
	{
		led_off(i);		
	}
	delay_ms(times);
	delay_ms(times);
	for(i = 0;i < num;i++)
	{
		led_on(i);
		delay_ms(times);
	}
	for(;i < 4;i++)
	{
		led_off(i);
		delay_ms(times);
	}
}
void display_battery_first(void)
{
	u16 v = 0;
	led_twinkle(4, 1, 200);
	v = get_battery_voltage();
	if(v > POWER80)      
	{
//		play_sound(11);
		display_battery_led_first(4, LED_INTERVAL_TIME);
	}
	else if(v > POWER50)  
	{
//		play_sound(12);
		display_battery_led_first(3, LED_INTERVAL_TIME);
	}
	else if(v > POWER15)  
	{
//		play_sound(13);
		display_battery_led_first(2, LED_INTERVAL_TIME);
	}
	else if(v > POWER5)  
	{
		play_sound(14);
		display_battery_led_first(1, LED_INTERVAL_TIME);
	}
	else                 
	{
		play_sound(15);
		display_battery_led_first(0, LED_INTERVAL_TIME);	
	}
}

#endif

void gener_shake_signal(u16 time)
{
	led_off(5);
	delay_ms(10);
	led_on(5);
	delay_ms(time);
	led_off(5);
}

/*******************************************END OF FILE******************************************/
