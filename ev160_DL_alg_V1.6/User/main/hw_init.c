/******************************************************************************
* �ļ���         : hw_init.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : *
* ����           : Ӳ����ʼ������
*
*******************************************************************************/

#include "hw_lib.h"

/*******************************************************************************
* ��������       : RCC_Configuration
* ����           : RCC���ã�����ϵͳ��ʱ�ӣ���ʹ��һЩģ���ʱ��
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
static void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCCϵͳ��λ�������ã� */
	RCC_DeInit();

	/* ����ʹ���ⲿʱ�ӣ�HSE ����·�� */
	RCC_HSEConfig(RCC_HSE_ON);

	/* �ȴ�ֱ��HSE׼������ */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
//	RCC_HSICmd(ENABLE);//ʹ��HSI 
	if (HSEStartUpStatus == SUCCESS)
	{
//		/* ʹ��FLASH�Ļ�����Ԥȡ���� */
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

//		/* FLASHʹ��2���ӳ����� */
//		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 

		/* PCLK1 = HCLK/4 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//9  72MHZ//HSE 8Mhz
	//	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);

		/* ʹ�� PLL */
		RCC_PLLCmd(ENABLE);

		/* �ȴ�ֱ��PLL׼������ */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		/* ѡ��PLL��Ϊϵͳ��ʱ��Դ */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* �ȴ�ֱ��PLL�ѳ�Ϊϵͳ��ʱ��Դ */
    while (RCC_GetSYSCLKSource() != 0x08);
	}
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//���Կ���Ϊ��ͨIO	
}

/*******************************************************************************
* ��������       : NVIC_Configuration
* ����           : NVIC���ã������жϵ���ڡ����ȼ���
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
} 

/*******************************************************************************
* ��������       : GPIO_Configuration
* ����           : �˿�����
* ����           : ��
* ����USART1		 ��TX��A9��RX��A10
* ����USART3		 ��TX��B10��RX��B11
* ����MCO				 ��MCO��A8
* ����ADC1			 ��CH0��A0��CH1��A1��CH2��A2��CH3��A3
* ����ADC1			 ��CH4��A4��CH5��A5��CH6��A6��CH7��A7
* ����ADC1			 ��CH8��B0��CH9��B1��CH14��C4��CH15��C5
* ����ADC1			 ��Battery: CH13��C3
* ����BatteryLED ��LED1��A15��LED2��B3��LED3��B4��LED4��B5��
* ����IO OUT     ��IO��C12
* ����IO shake   ��IO��B8
* ����IO interrupt ��IO��D2
* ����IO IN 		 ��	IO��C9
* ����detect door ��IO��C0,C1,C2,C13
* ����ֵ         : ��
*******************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);	
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//����USART1		 ��TX��A9��RX��A10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//����USART3		 ��TX��B10��RX��B11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;										 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	//usart4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;										 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	//usart5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;										 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	//mco
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����ADC1			 ��CH0��A0��CH1��A1��CH2��A2��CH3��A3
	//����ADC1			 ��CH4��A4��CH5��A5��CH6��A6��CH7��A7
	//����ADC1			 ��CH8��B0��CH9��B1��CH14��C4��CH15��C5
	//����ADC1			 ��Battery: CH13��C3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
  GPIO_Init(GPIOB, &GPIO_InitStructure);   	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
	//����BatteryLED ��LED1��A15��LED2��B3��LED3��B4��LED4��B5��
	//����IO shake   ��IO��B8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;										 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	//����IO OUT 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 
//	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
//	//����IO interrupt
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;										 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	//����detect door ��IO��C0,C1,C2,C13
	//����IO IN 		 ��	IO��C6,C7,C8,C9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);	
	GPIO_ResetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
//	//����detect door ��IO��B15,C6,C7,C8
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

/*******************************************************************************
* ��������       : MCO_Config
* ����           : ����MCO
* ����           : ��
* ����MCOʱ��Դ  ��RCC_MCO_PLLCLK_Div2
* ����ֵ         : ��
*******************************************************************************/
void MCO_Config(void)
{
	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //
}
/*******************************************************************************
* ��������       : Fun_configuration
* ����           : ��������
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
static void Fun_configuration(void)
{	
	MCO_Config();
//	systick_init();
	timer_init();
	USART3_Config();	//sim800					
	USART4_Config(); //����
	USART5_Config(); 
	sound_vol_set(30);
#if SERIAL_DEBUG			
	USART1_Config(); /*-----------�������-------------*/
#else
	//I2C_GPIO_Config();
//	I2C2_Init();
#endif	
	ADC1_Init();								/*-----------ADC���-------------*/	
	gpio_int_init();	
	#ifdef IWDG_FUN
	dog_init();
	dog_en();
	dog_eat();
	#endif
}
/*******************************************************************************
* ��������       : Fun_configuration
* ����           : ��������
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
void Data_Init(void)
{	
//	sim_test();
//	play_sound(16);
	led_on(4);
//	door_state_init();
}
/*******************************************************************************
* ��������       : HW_Configuration
* ����           : STM32��Ƭ��Ӳ����HardWare�������ú���
* ����           : ��
* ����ֵ         : ��
*******************************************************************************/
void HW_Configuration(void)
{
	/* ʱ�ӵ����� */
	RCC_Configuration();
    
//    if (FLASH_GetReadOutProtectionStatus() == RESET) 
//    {
//        FLASH_Unlock();
//        FLASH_ReadOutProtection(ENABLE); 
//    }
	
	/* NVIC���� */
	NVIC_Configuration();
	
	/* �˿ڵ����� */
	GPIO_Configuration();		
	
	/* �������� */
	Fun_configuration();
	
	
}

/*******************************************END OF FILE******************************************/
