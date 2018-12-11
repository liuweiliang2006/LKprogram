/******************************************************************************
* 文件名         : api_door.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 接口程序，检测开关门状态
*
*******************************************************************************/
#include "hw_lib.h"

bool is_door_allclosed(void)
{
	if(is_door_closed(5) == TRUE) return TRUE;
	if(is_door_closed(0)&&is_door_closed(1)&&is_door_closed(2)&&is_door_closed(3))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
static u16 open_time = 0, close_time = 0;
DOOR_STATE get_door_state(void)
{
	while(1)
	{
		if(is_door_allclosed() == TRUE)//关门
		{
			close_time = 0;
			open_time++;
			delay_ms(10);
			if(open_time == 20)//200ms
			{
				open_time = 0;				
				return DOOR_CLOSE;
			}
		}
		else//开门
		{
			open_time = 0;
			close_time++;
			delay_ms(10);
			if(close_time == 20)//200ms
			{
				close_time = 0;
				return DOOR_OPEN;
			}
		}
	}
}
DOOR_STATE door_now_state = DOOR_OPEN;
void door_state_init(void)
{
	door_now_state = get_door_state();
}
void detect_door(void)
{
	if(get_door_state() == DOOR_OPEN)
	{
		if(door_now_state != DOOR_OPEN)
		{
			door_now_state = DOOR_OPEN;
//			play_sound(7);//7：开车门
		}

		timer_led_twinkle(4, 0xffff, 800);
	}
	else
	{
		if(door_now_state != DOOR_CLOSE)
		{
			door_now_state = DOOR_CLOSE;
//			play_sound(8);//8：关车门
			delay_ms(1200);	
			memset(AD_store_final,0,CHANNEL*FILTER_CNT*sizeof(u16));
			trigger_status = NOTRIGGER;
			Admit_SampleAD = TRUE;	
		}
		
		timer_led_twinkle_off();
		display_battery();	
	}
}
DOOR_STATE detect_door2(void)
{
	if(get_door_state() == DOOR_OPEN)
	{
		return DOOR_OPEN;
	}
	else
	{
		if(door_now_state != DOOR_CLOSE)
		{			
			return DOOR_OPEN;
		}		
		return DOOR_CLOSE;
	}
}

bool test_door[4] = {FALSE};
void door_detect_test(void)
{
	u16 i;	
	for(i = 0;i < 4;i++)
	{
		test_door[i] = is_door_closed(i);
	}
}



/*******************************************END OF FILE******************************************/

