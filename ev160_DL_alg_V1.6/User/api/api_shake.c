/******************************************************************************
* �ļ���         : api_shake.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : *
* ����           : �ӿڳ��򣬲�����ģ���źţ�����360ȫ��
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

