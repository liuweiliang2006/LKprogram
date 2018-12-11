/******************************************************************************
* 文件名         : api_sendmessage.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 接口程序，串口发送数据接口程序
*
*******************************************************************************/

#include "hw_lib.h"

#if	SERIAL_DEBUG

void Send_debug_message(void)
{
	int i,j;
	char high_1,low_1,high_2,low_2;

	int ST = 85;	//55
	int TS = 170;	//AA

	high_1 = (char)(0);
	low_1 = (char)((ST)&0x00ff);
//	printf("%c",low_1);
//	printf("%c",high_1);
//	
	printf("%c%c",low_1,high_1);

	for(i=0;i<CHANNEL;i++)
	{
//		high_1 = (char)((Threshold_Max[i]>>8)&0x00ff);
//		low_1 = (char)((Threshold_Max[i])&0x00ff);		
//		high_2 = (char)((Threshold_Min[i]>>8)&0x00ff);
//		low_2 = (char)((Threshold_Min[i])&0x00ff);		
//		high_1 = 0xff;
//		low_1 = data_process_result[i];		
//		high_2 = 0xff;
//		low_2 = 0xff;	
		high_1 = 0xff;
		low_1 = 0xff;		
		high_2 = 0xff;
		low_2 = 0xff;	
		printf("%c%c%c%c",low_1,high_1,low_2,high_2);
	}

	for(i=0;i<FILTER_CNT;i++)
	{
		for(j = 0;j < CHANNEL;j++)
		{
			high_1 = (char)((AD_store_final[j][i]>>8)&0x00ff);
			low_1 = (char)((AD_store_final[j][i])&0x00ff);
			printf("%c%c",low_1,high_1);
		}
	}

	high_1 = (char)(0);
	low_1 = (char)((TS)&0x00ff);	
	printf("%c%c",low_1,high_1);

}

#endif


/*******************************************END OF FILE******************************************/


