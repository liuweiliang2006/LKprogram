/******************************************************************************
* 文件名         : api_shake.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 接口程序，产生震动模块信号，控制360全景
*
*******************************************************************************/
#include "hw_lib.h"

volatile u32 shake_time = 0;
volatile u32 shake_counter = 0;
volatile u8 begin_shake = 0;
void shake_time_process(void)
{
	if(begin_shake == 1)
	{
		shake_counter++;
		SHAKE_BEGING;
		if(shake_counter == shake_time)
		{
			SHAKE_END;
			begin_shake = 0;
		}
	}
}
void gener_shake(u32 time)
{
	shake_time = time;
	shake_counter = 0;
	begin_shake = 1;
}

/*******************************************END OF FILE******************************************/

