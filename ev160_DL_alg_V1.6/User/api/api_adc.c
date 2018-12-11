/******************************************************************************
* 文件名         : api_adc.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 接口程序，adc采集数据接口程序
*
*******************************************************************************/

#include "hw_lib.h"

/*--------------ADC相关-------------*/
TriggerStatus trigger_status = NOTRIGGER;		

u16 AD_value[CHANNEL];//AD采集DMA缓存
u16 Back_Average[CHANNEL];//底燥均值
u16 backnoise[CHANNEL]; //底燥最大偏移值
u16 Threshold_Max[CHANNEL],Threshold_Min[CHANNEL];//AD触发门限
u16 AD_store_final[CHANNEL][FILTER_CNT] = {0};//最终给用户的AD数据	

static u16 AD_store_temp[CHANNEL][AD_TEMP_CNT]; //AD临时存储
static u32 AD_sum[CHANNEL]={0};										 //AD背景采样时需要的总和
static u16 AD_min[CHANNEL]={0},AD_max[CHANNEL]={0}; //AD最小、最大值

bool Admit_SampleAD = TRUE;
static bool Finished_BackSampl = FALSE;					   //=FALSE:为背景采样；=TRUE：为非背景采样
static bool Begin_BackSampl = FALSE;
static u16 count_sampling = 0;											 //AD四个通道采样计数：进入DMA的次数
void dma1_irq_adc_process(void)
{
	/*--------------ADC相关-------------*/
    register u16 i,j;
		u16 AD_read_value[CHANNEL];

	for(i = 0;i < CHANNEL;i++) AD_read_value[i] = AD_value[i];
#ifdef DEBUGTEST
	
#else
	
#ifdef 	AUTOSAMPLING	
	if(trigger_status == NOTRIGGER)	//非预触发只执行这段  32
	{			
		for(i = 0;i < CHANNEL;i++) 
			AD_store_final[i][count_sampling] = AD_read_value[i]; 
		//用于存储数据的计数  1~32~256
		count_sampling+=1;	
		if(count_sampling >= FILTER_CNT){		
			count_sampling = 0 ;	
			trigger_status = TRIGGERED;	
		}					
	}
#else
	if(Admit_SampleAD)// 允许采样操作
	{
		if(Finished_BackSampl)// 背景采样已完成
		{
			if(trigger_status == NOTRIGGER)	// 未触发
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
			else if(trigger_status == PRETRIGGER)  // 预触发
			{
				for(i = 0;i < CHANNEL;i++) AD_store_final[i][count_sampling] = AD_read_value[i];
				count_sampling++;
				if(count_sampling == FILTER_CNT) // 采样完成
				{
					count_sampling = 0;
					trigger_status = TRIGGERED;
					Admit_SampleAD = FALSE;// 采样数据处理完成前不允许重新采样
				}
			}
			
		}
		else//背景采样未完成
		{
			if(Begin_BackSampl)
			{
				//累加 
				for(i = 0;i < CHANNEL;i++) AD_sum[i] += AD_read_value[i];
				//找极值
				for(i = 0;i < CHANNEL;i++){
					if(AD_read_value[i] < AD_min[i]) AD_min[i] = AD_read_value[i];
					else if(AD_read_value[i] > AD_max[i]) AD_max[i] = AD_read_value[i];	
				}
				count_sampling++;
				if(count_sampling == BackSampl_CNT){ 
					//背景噪声计算
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
				//累加初始
				for(i = 0;i < CHANNEL;i++) AD_sum[i] += AD_read_value[i];
				//极值初始		
				for(i = 0;i < CHANNEL;i++){
					AD_min[i] = AD_read_value[i];
					AD_max[i] = AD_read_value[i];
				}
				//背景采样开始
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








