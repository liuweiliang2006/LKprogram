/******************************************************************************
* �ļ���         : api_result..c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : *
* ����           : �������
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





