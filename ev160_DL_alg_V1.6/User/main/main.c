/******************************************************************************
* 文件名         : main.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 主程序
*
*******************************************************************************/

#include "hw_lib.h"
u32 door_times = 0;
/*******************************************************************************
* 函数名称       : main
* 描述           : 主函数
* 输入           : 无
* 返回值         : 无
*******************************************************************************/
//uint8_t Rxflag=0;
//uint8_t ucTemp;
//static uint8_t ucPackNum=1;  //package number 1-255

uint16_t ucSalvePackLen=0; //从机发送的包长度，包括帧头及数据

uint8_t senddata[6144]={0};	
uint8_t ucReciveBuffer[512+8];        //中断接收数据包，一个包内最大长度为数据512+包头8
uint8_t ucSetThreshold[18];           //18字节域值存放处
uint8_t ucAlarmtest[4]={1,2,3,4};     //报警测试数据

uint8_t ucSlaveResponseAlarm[8];      //从机响应报警命令接收包
uint8_t ucSlaveResponseCarState[16];  //从机响应主机获取车辆状态命令接收包
uint8_t ucSlaveResponseSendData[8];   //从机响应主机发送数据命令接收包

u32 force_data = 0;
u32 force_data_sum = 0;



CarThresholdInfo_TYPE  g_tCarThreshold = {0};
CarDoorStateInfo_TYPE  g_tCarDoorState = {0};
CarStateInfo_TYPE g_tCarState={0};

//车门状态、AD阈值处发状态、算法处理标识初始状态

uint8_t g_ucCarDoorFlag=1;							//车门状态标识 1表示开 0表示关
uint8_t g_ucADCTriggerFlag=0;						//ADC阈值触发标识
uint8_t g_ucAlgorithmFlag=0;						//算法处理标识
uint8_t g_ucMasterSendFlag=0;						//发送标识  0不具备发送条件 1具备发送条件
uint8_t g_ucCarLockFlag=0;							//车门锁状态标识 0表示无动作 1表示有动作
uint8_t g_ucCarWinFlag=0;								//车窗状态标识 0表示无动作 1表示有动作
uint8_t g_ucCarTrunkFlag=0;							//后备箱状态标识 0表示无动作 1表示有动作
uint8_t g_ucCarACCFlag=0;								//ACC上电状态 0表示ACC断电 1表示ACC上电
uint8_t g_ucThresholdFlag=0;						//阈值设置标识符 1表示阈值有更新 0表示阈值没有更新
uint8_t g_ucGetCarStateresponse=0;			//从机是否有响应getCarStater指令

int main(void)
{
	DATA_JUGE_Type * str_data_juge;
	delay_100us(2000); //防止上电不稳定干扰
	HW_Configuration();	
	DelayInit();
//	g_tCarDoorState.DOORBITS.DOOR=OPEN;
//	g_tCarDoorState.DOORBITS.RFDOOR=1;
//	g_tCarDoorState.DOORBITS.LFDOOR=1;
//	g_tCarDoorState.DOORBITS.LBDOOR=1;
//	g_tCarDoorState.ucYear=18;
//	g_tCarDoorState.ucMonth=12;
//	g_tCarDoorState.ucDay=6;
//	g_tCarDoorState.ucHour=16;
//	g_tCarDoorState.ucMinute=30;
//	g_tCarDoorState.ucSecond=10;	
//	len=sizeof(g_tCarDoorState);
//	memcpy(test,&g_tCarDoorState,sizeof(g_tCarDoorState));
	display_battery_first();	
	/* 数据初始化 */
	Data_Init();	
	debug_get_adjust();
	
  /*更新车门状态标识*/
	Master_Send_GetCarState_data();  	
	/*延时待车体稳定*/
	DelayMs(1000);
	DelayMs(1000);

	
	while (TRUE)
	{	
		/*处理从机发送的数据*/
		if(ucSalvePackLen!=0) 
		{
			UcAnalysis_RecSlave_Data();			
			ucSalvePackLen=0;
		}
		

#ifdef DEBUGTEST
//		test_flash_drv();
//	  sim_test();
//		send_sms(1);
//	  gener_shake(100);
//    door_detect_test();
//		gener_shake_signal(100);
//		debug_uart_test_data();
		test_generate_data();
//		data_process_test();
//		sound_test();
//	  data_process_test();
		while(1);		
#else
	#ifdef 	AUTOSAMPLING	
		if(trigger_status == TRIGGERED)
		{		
			Send_debug_message();		
			
			display_battery();
			memset(AD_store_final,0,CHANNEL*FILTER_CNT*sizeof(u16));					
			delay_ms(5000);
			trigger_status = NOTRIGGER;
		}
	#else
		#ifdef MAINRUN	
		
		
		if(g_ucCarDoorFlag==CLOSE)
		{
			/*车门状态为关时，处理AD采样及算法等相关工作*/
			if(trigger_status == TRIGGERED)
			{
				/*AD阈值触发后，即使在进入DMA中断，也不允许进行采样处理工作*/
				Admit_SampleAD = FALSE;
				
				/*有数据触发灯闪烁*/
				timer_led_twinkle_off();
				timer_led_twinkle(4, 0xffff, 100);
				Send_debug_message();
				/*将数据传递给算法，并将用结构体指针str_data_juge接收算法处理结果*/
				str_data_juge=data_process_lowdetect((&AD_store_final[0]), CHANNEL);
//				data_process_lowdetect_result(data_process_lowdetect((&AD_store_final[0]), CHANNEL));
				
				/*确认算法计算是否发生碰撞事件*/
				if(str_data_juge->report_result!=NONE)  
				{
					//发生碰撞事件
					Master_Send_GetCarState_data();
					 
					/*等待从机对车门当前状态进行响应，并汇报给主机*/
					while(g_ucGetCarStateresponse!=0)
					{
						if(ucSalvePackLen!=0) 
						{
							UcAnalysis_RecSlave_Data();			
							ucSalvePackLen=0;
						}
					}
//					DelayMs(500); //从机响应时间，更新g_ucCarDoorFlag标志位
					if(g_ucCarDoorFlag==CLOSE)
					{ 
						/*记录算法的计算结果*/
						ucAlarmtest[0]=str_data_juge->report_pos>>8;
						ucAlarmtest[1]=str_data_juge->report_pos&0xFF;
						ucAlarmtest[2]=str_data_juge->max1_i;
						ucAlarmtest[3]=str_data_juge->max2_i;
						
						/*发送报警数据*/
						Master_Send_Alarm_Data(ucAlarmtest);
						
						/*发送上报数据*/
						memcpy(senddata,AD_store_final[str_data_juge->max1_i],sizeof(AD_store_final[str_data_juge->max1_i]));
						Master_Send_Data(senddata,6144);						
					}
				}
				/*/显示电池电量*/
				display_battery();
				
				/*数据buf清0*/
				memset(AD_store_final,0,CHANNEL*FILTER_CNT*sizeof(u16));
				trigger_status = NOTRIGGER;
				Admit_SampleAD = TRUE;
				
				/*数据处理结束，关闭灯闪烁*/
				timer_led_twinkle_off();
			}
		}
		else
		{
			/*车门状态为开时，不进行处理AD采样及算法等相关工作，清空AD触发及存储AD_store_final，令AD继续工作采样*/
			memset(AD_store_final,0,CHANNEL*FILTER_CNT*sizeof(u16));
			trigger_status = NOTRIGGER;
			Admit_SampleAD = TRUE;
		}
		#endif		
	#endif
#endif
	}
}

/*******************************************END OF FILE******************************************/
