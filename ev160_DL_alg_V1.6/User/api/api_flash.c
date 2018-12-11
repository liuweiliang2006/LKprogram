/******************************************************************************
* 文件名         : drv_flash.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，flash
*
*******************************************************************************/

#include "hw_lib.h"	

typedef struct FLASH_HEAD
{
	u8 cover_times;//该位置覆盖次数,从1开始累计，至0xff为止
	u8 reverse1;
	u16 num;//数据组数，从1开始累计，至0xffff为止
	u16 channel;
	u16 cnt;
	u32 time_s;
	u8 reverse2; //result
	u8 reverse3; //num
	u8 reverse4;
	u8 reverse5;
}FLASH_HEAD_Type;	

FLASH_HEAD_Type flash_header;
FLASH_HEAD_Type * flash_header_ptr;

#define GROUP_NUM_MAX							50//存储的数据组数
#define GROUP_SIZE								3//每组数据占用的flash页数

u16 flash_buf_temp[800];
u16 pre_num;//上一组数据编号
u16 group_num;//本组数据存储组数位置
void store_get_group_num(void)
{
	u16 i;
	pre_num = 0;
	for(i = 0;i < GROUP_NUM_MAX;i++)
	{
		flash_header_ptr = (FLASH_HEAD_Type *)(SECTOR_ADR(1) + i*16);
		if(flash_header_ptr->num == 0xffff)
		{
			pre_num = i;
			return;
		}
		if(flash_header_ptr->num > pre_num)
		{
			pre_num = flash_header_ptr->num;
		}
		else
		{
			return;
		}	
	}	
	return;
}

void store_data(u16 * buf)
{
	store_get_group_num();
	if(pre_num != 0)
	{
		group_num = pre_num%GROUP_NUM_MAX;	
	}
	else
	{
		group_num = 0;
	}
	
	flash_write_sector(group_num*GROUP_SIZE + 2, buf + 0);
	flash_write_sector(group_num*GROUP_SIZE + 3, buf + STM_SECTOR_SIZE_HALF_WORD);
	flash_write_sector(group_num*GROUP_SIZE + 4, buf + STM_SECTOR_SIZE_HALF_WORD*2);
	
	flash_read_sector(1, flash_buf_temp);
	
	flash_header_ptr = (FLASH_HEAD_Type *)((u8 *)flash_buf_temp + group_num*16);
	if(flash_header_ptr->cover_times == 0xff)
	{
		flash_header_ptr->cover_times = 1;
	}
	else if(flash_header_ptr->cover_times != 0xfe)
	{
		flash_header_ptr->cover_times += 1;
	}
	flash_header_ptr->num = pre_num + 1;
	flash_header_ptr->channel = CHANNEL;
	flash_header_ptr->cnt = FILTER_CNT;
	flash_header_ptr->time_s = get_timer_s();
//	flash_header_ptr->reverse2 = data_process_result[0];
//	flash_header_ptr->reverse3 = data_process_result[1];
	
	flash_write_sector(1, flash_buf_temp);
}



/*******************************************END OF FILE******************************************/




