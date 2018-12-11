/******************************************************************************
* 文件名         : hw_init.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 硬件初始化程序
*
*******************************************************************************/

#include "hw_lib.h"

/*******************************************************************************
* 函数名称       : RCC_Configuration
* 描述           : RCC配置，配置系统的时钟，并使能一些模块的时钟
* 输入           : 无
* 返回值         : 无
*******************************************************************************/
static void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC系统复位（调试用） */
	RCC_DeInit();

	/* 由于使用外部时钟，HSE 被旁路掉 */
	RCC_HSEConfig(RCC_HSE_ON);

	/* 等待直到HSE准备就绪 */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
//	RCC_HSICmd(ENABLE);//使能HSI 
	if (HSEStartUpStatus == SUCCESS)
	{
//		/* 使能FLASH的缓冲区预取功能 */
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

//		/* FLASH使用2个延迟周期 */
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

		/* 使能 PLL */
		RCC_PLLCmd(ENABLE);

		/* 等待直到PLL准备就绪 */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		/* 选中PLL作为系统的时钟源 */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* 等待直到PLL已成为系统的时钟源 */
    while (RCC_GetSYSCLKSource() != 0x08);
	}
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//调试口作为普通IO	
}

/*******************************************************************************
* 函数名称       : NVIC_Configuration
* 描述           : NVIC配置，配置中断的入口、优先级等
* 输入           : 无
* 返回值         : 无
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
* 函数名称       : GPIO_Configuration
* 描述           : 端口配置
* 输入           : 无
* 配置USART1		 ：TX：A9，RX：A10
* 配置USART3		 ：TX：B10，RX：B11
* 配置MCO				 ：MCO：A8
* 配置ADC1			 ：CH0：A0，CH1：A1，CH2：A2，CH3：A3
* 配置ADC1			 ：CH4：A4，CH5：A5，CH6：A6，CH7：A7
* 配置ADC1			 ：CH8：B0，CH9：B1，CH14：C4，CH15：C5
* 配置ADC1			 ：Battery: CH13：C3
* 配置BatteryLED ：LED1：A15，LED2：B3，LED3：B4，LED4：B5，
* 配置IO OUT     ：IO：C12
* 配置IO shake   ：IO：B8
* 配置IO interrupt ：IO：D2
* 配置IO IN 		 ：	IO：C9
* 配置detect door ：IO：C0,C1,C2,C13
* 返回值         : 无
*******************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);	
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//配置USART1		 ：TX：A9，RX：A10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//配置USART3		 ：TX：B10，RX：B11
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
	//配置ADC1			 ：CH0：A0，CH1：A1，CH2：A2，CH3：A3
	//配置ADC1			 ：CH4：A4，CH5：A5，CH6：A6，CH7：A7
	//配置ADC1			 ：CH8：B0，CH9：B1，CH14：C4，CH15：C5
	//配置ADC1			 ：Battery: CH13：C3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);   	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
	//配置BatteryLED ：LED1：A15，LED2：B3，LED3：B4，LED4：B5，
	//配置IO shake   ：IO：B8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;										 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	//配置IO OUT 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 
//	GPIO_ResetBits(GPIOC, GPIO_Pin_12);
//	//配置IO interrupt
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;										 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	//配置detect door ：IO：C0,C1,C2,C13
	//配置IO IN 		 ：	IO：C6,C7,C8,C9
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
//	//配置detect door ：IO：B15,C6,C7,C8
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
* 函数名称       : MCO_Config
* 描述           : 配置MCO
* 输入           : 无
* 配置MCO时钟源  ：RCC_MCO_PLLCLK_Div2
* 返回值         : 无
*******************************************************************************/
void MCO_Config(void)
{
	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //
}
/*******************************************************************************
* 函数名称       : Fun_configuration
* 描述           : 功能配置
* 输入           : 无
* 返回值         : 无
*******************************************************************************/
static void Fun_configuration(void)
{	
	MCO_Config();
//	systick_init();
	timer_init();
	USART3_Config();	//sim800					
	USART4_Config(); //语音
	USART5_Config(); 
	sound_vol_set(30);
#if SERIAL_DEBUG			
	USART1_Config(); /*-----------调试相关-------------*/
#else
	//I2C_GPIO_Config();
//	I2C2_Init();
#endif	
	ADC1_Init();								/*-----------ADC相关-------------*/	
	gpio_int_init();	
	#ifdef IWDG_FUN
	dog_init();
	dog_en();
	dog_eat();
	#endif
}
/*******************************************************************************
* 函数名称       : Fun_configuration
* 描述           : 功能配置
* 输入           : 无
* 返回值         : 无
*******************************************************************************/
void Data_Init(void)
{	
//	sim_test();
//	play_sound(16);
	led_on(4);
//	door_state_init();
}
/*******************************************************************************
* 函数名称       : HW_Configuration
* 描述           : STM32单片机硬件（HardWare）的配置函数
* 输入           : 无
* 返回值         : 无
*******************************************************************************/
void HW_Configuration(void)
{
	/* 时钟的配置 */
	RCC_Configuration();
    
//    if (FLASH_GetReadOutProtectionStatus() == RESET) 
//    {
//        FLASH_Unlock();
//        FLASH_ReadOutProtection(ENABLE); 
//    }
	
	/* NVIC配置 */
	NVIC_Configuration();
	
	/* 端口的配置 */
	GPIO_Configuration();		
	
	/* 功能配置 */
	Fun_configuration();
	
	
}

/*******************************************END OF FILE******************************************/
