/******************************************************************************
* �ļ���         : main.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : *
* ����           : ������
*
*******************************************************************************/

#include "hw_lib.h"
u32 door_times = 0;
/*******************************************************************************
* ��������       : main
* ����           : ������
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
//uint8_t Rxflag=0;
//uint8_t ucTemp;
//static uint8_t ucPackNum=1;  //package number 1-255

uint16_t ucSalvePackLen=0; //�ӻ����͵İ����ȣ�����֡ͷ������

uint8_t senddata[6144]={0};	
uint8_t ucReciveBuffer[512+8];        //�жϽ������ݰ���һ��������󳤶�Ϊ����512+��ͷ8
uint8_t ucSetThreshold[18];           //18�ֽ���ֵ��Ŵ�
uint8_t ucAlarmtest[4]={1,2,3,4};     //������������

uint8_t ucSlaveResponseAlarm[8];      //�ӻ���Ӧ����������հ�
uint8_t ucSlaveResponseCarState[16];  //�ӻ���Ӧ������ȡ����״̬������հ�
uint8_t ucSlaveResponseSendData[8];   //�ӻ���Ӧ������������������հ�

u32 force_data = 0;
u32 force_data_sum = 0;



CarThresholdInfo_TYPE  g_tCarThreshold = {0};
CarDoorStateInfo_TYPE  g_tCarDoorState = {0};
CarStateInfo_TYPE g_tCarState={0};

//����״̬��AD��ֵ����״̬���㷨�����ʶ��ʼ״̬

uint8_t g_ucCarDoorFlag=1;							//����״̬��ʶ 1��ʾ�� 0��ʾ��
uint8_t g_ucADCTriggerFlag=0;						//ADC��ֵ������ʶ
uint8_t g_ucAlgorithmFlag=0;						//�㷨�����ʶ
uint8_t g_ucMasterSendFlag=0;						//���ͱ�ʶ  0���߱��������� 1�߱���������
uint8_t g_ucCarLockFlag=0;							//������״̬��ʶ 0��ʾ�޶��� 1��ʾ�ж���
uint8_t g_ucCarWinFlag=0;								//����״̬��ʶ 0��ʾ�޶��� 1��ʾ�ж���
uint8_t g_ucCarTrunkFlag=0;							//����״̬��ʶ 0��ʾ�޶��� 1��ʾ�ж���
uint8_t g_ucCarACCFlag=0;								//ACC�ϵ�״̬ 0��ʾACC�ϵ� 1��ʾACC�ϵ�
uint8_t g_ucThresholdFlag=0;						//��ֵ���ñ�ʶ�� 1��ʾ��ֵ�и��� 0��ʾ��ֵû�и���
uint8_t g_ucGetCarStateresponse=0;			//�ӻ��Ƿ�����ӦgetCarStaterָ��

int main(void)
{
	DATA_JUGE_Type * str_data_juge;
	delay_100us(2000); //��ֹ�ϵ粻�ȶ�����
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
	/* ���ݳ�ʼ�� */
	Data_Init();	
	debug_get_adjust();
	
  /*���³���״̬��ʶ*/
	Master_Send_GetCarState_data();  	
	/*��ʱ�������ȶ�*/
	DelayMs(1000);
	DelayMs(1000);

	
	while (TRUE)
	{	
		/*����ӻ����͵�����*/
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
			/*����״̬Ϊ��ʱ������AD�������㷨����ع���*/
			if(trigger_status == TRIGGERED)
			{
				/*AD��ֵ�����󣬼�ʹ�ڽ���DMA�жϣ�Ҳ��������в���������*/
				Admit_SampleAD = FALSE;
				
				/*�����ݴ�������˸*/
				timer_led_twinkle_off();
				timer_led_twinkle(4, 0xffff, 100);
				Send_debug_message();
				/*�����ݴ��ݸ��㷨�������ýṹ��ָ��str_data_juge�����㷨������*/
				str_data_juge=data_process_lowdetect((&AD_store_final[0]), CHANNEL);
//				data_process_lowdetect_result(data_process_lowdetect((&AD_store_final[0]), CHANNEL));
				
				/*ȷ���㷨�����Ƿ�����ײ�¼�*/
				if(str_data_juge->report_result!=NONE)  
				{
					//������ײ�¼�
					Master_Send_GetCarState_data();
					 
					/*�ȴ��ӻ��Գ��ŵ�ǰ״̬������Ӧ�����㱨������*/
					while(g_ucGetCarStateresponse!=0)
					{
						if(ucSalvePackLen!=0) 
						{
							UcAnalysis_RecSlave_Data();			
							ucSalvePackLen=0;
						}
					}
//					DelayMs(500); //�ӻ���Ӧʱ�䣬����g_ucCarDoorFlag��־λ
					if(g_ucCarDoorFlag==CLOSE)
					{ 
						/*��¼�㷨�ļ�����*/
						ucAlarmtest[0]=str_data_juge->report_pos>>8;
						ucAlarmtest[1]=str_data_juge->report_pos&0xFF;
						ucAlarmtest[2]=str_data_juge->max1_i;
						ucAlarmtest[3]=str_data_juge->max2_i;
						
						/*���ͱ�������*/
						Master_Send_Alarm_Data(ucAlarmtest);
						
						/*�����ϱ�����*/
						memcpy(senddata,AD_store_final[str_data_juge->max1_i],sizeof(AD_store_final[str_data_juge->max1_i]));
						Master_Send_Data(senddata,6144);						
					}
				}
				/*/��ʾ��ص���*/
				display_battery();
				
				/*����buf��0*/
				memset(AD_store_final,0,CHANNEL*FILTER_CNT*sizeof(u16));
				trigger_status = NOTRIGGER;
				Admit_SampleAD = TRUE;
				
				/*���ݴ���������رյ���˸*/
				timer_led_twinkle_off();
			}
		}
		else
		{
			/*����״̬Ϊ��ʱ�������д���AD�������㷨����ع��������AD�������洢AD_store_final����AD������������*/
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
