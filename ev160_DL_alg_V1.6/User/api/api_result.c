/******************************************************************************
* 文件名         : api_result..c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 播报结果
*
*******************************************************************************/

#include "hw_lib.h"

void data_process_lowdetect_result(DATA_JUGE_Type * str_data_juge)
{
	if(str_data_juge->report_result != NONE)
	{ 
		play_sound(str_data_juge->report_pos);
		if(str_data_juge->report_result == CRASH)
		{
			play_sound(9);
		}
		else
		{
			play_sound(10);
		}
	}
//	store_data((u16 *)AD_store_final);

}

/*******************************************END OF FILE******************************************/





