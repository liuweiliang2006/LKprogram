/******************************************************************************
* �ļ���         : api_adc.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : *
* ����           : �ӿڳ���adc�ɼ����ݽӿڳ���
*
*******************************************************************************/

#include "hw_lib.h"

/*--------------ADC���-------------*/
TriggerStatus trigger_status = NOTRIGGER;		

u16 AD_value[CHANNEL];//AD�ɼ�DMA����
u16 Back_Average[CHANNEL];//�����ֵ
u16 backnoise[CHANNEL]; //�������ƫ��ֵ
u16 Threshold_Max[CHANNEL],Threshold_Min[CHANNEL];//AD��������
u16 AD_store_final[CHANNEL][FILTER_CNT] = {0};//���ո��û���AD����	

static u16 AD_store_temp[CHANNEL][AD_TEMP_CNT]; //AD��ʱ�洢
static u32 AD_sum[CHANNEL]={0};										 //AD��������ʱ��Ҫ���ܺ�
static u16 AD_min[CHANNEL]={0},AD_max[CHANNEL]={0}; //AD��С�����ֵ

bool Admit_SampleAD = TRUE;
static bool Finished_BackSampl = FALSE;					   //=FALSE:Ϊ����������=TRUE��Ϊ�Ǳ�������
static bool Begin_BackSampl = FALSE;
static u16 count_sampling = 0;											 //AD�ĸ�ͨ����������������DMA�Ĵ���
void dma1_irq_adc_process(void)
{
	/*--------------ADC���-------------*/
    register u16 i,j;
		u16 AD_read_value[CHANNEL];

	for(i = 0;i < CHANNEL;i++) AD_read_value[i] = AD_value[i];
#ifdef DEBUGTEST
	
#else
	
#ifdef 	AUTOSAMPLING	
	if(trigger_status == NOTRIGGER)	//��Ԥ����ִֻ�����  32
	{			
		for(i = 0;i < CHANNEL;i++) 
			AD_store_final[i][count_sampling] = AD_read_value[i]; 
		//���ڴ洢���ݵļ���  1~32~256
		count_sampling+=1;	
		if(count_sampling >= FILTER_CNT){		
			count_sampling = 0 ;	
			trigger_status = TRIGGERED;	
		}					
	}
#else
	if(Admit_SampleAD)// �����������
	{
		if(Finished_BackSampl)// �������������
		{
			if(trigger_status == NOTRIGGER)	// δ����
			{
				for(i = 0;i < CHANNEL;i++) AD_store_temp[i][count_sampling] = AD_read_value[i]; 
				count_sampling++;
				if(count_sampling == AD_TEMP_CNT) count_sampling = 0;
				for(i = 0;i < CHANNEL;i++)
				{
					if((AD_read_value[i] > Threshold_Max[i]) || (AD_read_value[i] < Threshold_Min[i]))
					{
						for(j = 0;j < AD_TEMP_CNT;j++)
						{
							for(i = 0;i < CHANNEL;i++) 
							AD_store_final[i][j] = AD_store_temp[i][count_sampling];
							count_sampling++;
							if(count_sampling == AD_TEMP_CNT)
								count_sampling = 0;
						}
						count_sampling = AD_TEMP_CNT;
						trigger_status = PRETRIGGER;
						break;
					}
				}				
			}		
			else if(trigger_status == PRETRIGGER)  // Ԥ����
			{
				for(i = 0;i < CHANNEL;i++) AD_store_final[i][count_sampling] = AD_read_value[i];
				count_sampling++;
				if(count_sampling == FILTER_CNT) // �������
				{
					count_sampling = 0;
					trigger_status = TRIGGERED;
					Admit_SampleAD = FALSE;// �������ݴ������ǰ���������²���
				}
			}
			
		}
		else//��������δ���
		{
			if(Begin_BackSampl)
			{
				//�ۼ� 
				for(i = 0;i < CHANNEL;i++) AD_sum[i] += AD_read_value[i];
				//�Ҽ�ֵ
				for(i = 0;i < CHANNEL;i++){
					if(AD_read_value[i] < AD_min[i]) AD_min[i] = AD_read_value[i];
					else if(AD_read_value[i] > AD_max[i]) AD_max[i] = AD_read_value[i];	
				}
				count_sampling++;
				if(count_sampling == BackSampl_CNT){ 
					//������������
					for(i=0;i<CHANNEL;i++){
						Back_Average[i] = (u16)(AD_sum[i]/BackSampl_CNT);								
						backnoise[i] = (AD_max[i] - Back_Average[i]) > (Back_Average[i] - AD_min[i])?(AD_max[i] - Back_Average[i]):(Back_Average[i] - AD_min[i]);									
						Threshold_Min[i] = Back_Average[i] - MULTIPLE*backnoise[i] - TOLERANCE;
						Threshold_Max[i] = Back_Average[i] + MULTIPLE*backnoise[i] + TOLERANCE;
					}									
					count_sampling = 0;
					Finished_BackSampl = TRUE;
				}		
			}
			else
			{
				//�ۼӳ�ʼ
				for(i = 0;i < CHANNEL;i++) AD_sum[i] += AD_read_value[i];
				//��ֵ��ʼ		
				for(i = 0;i < CHANNEL;i++){
					AD_min[i] = AD_read_value[i];
					AD_max[i] = AD_read_value[i];
				}
				//����������ʼ
				count_sampling = 1;
				Begin_BackSampl = TRUE;
			}	
		}
	}
#endif

#endif		
	
}

void adc_set_threshold(u16 tol)
{
	u8 i;
	Admit_SampleAD = FALSE;
	trigger_status = NOTRIGGER;	
	delay_100us(500);//50ms
	trigger_status = NOTRIGGER;
	Admit_SampleAD = FALSE;
	
	for(i=0;i<CHANNEL;i++){								
		Threshold_Min[i] = Back_Average[i] - MULTIPLE*backnoise[i] - tol;
		Threshold_Max[i] = Back_Average[i] + MULTIPLE*backnoise[i] + tol;
	}	
}

void adc_adjust(u32 adjust)
{
	u16 i;
	while(Finished_BackSampl == FALSE);
	for(i=0;i<CHANNEL;i++){
		Threshold_Min[i] -= adjust;
		Threshold_Max[i] += adjust;
	}
}
/*******************************************END OF FILE******************************************/








