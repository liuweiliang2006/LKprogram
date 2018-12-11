/******************************************************************************
* 文件名         : drv_detect.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，检测IO口电平相关
*
*******************************************************************************/
#include "hw_lib.h"

//detect door ：IO：C0,C1,C2,C13
//4, C9:SMS
//5, C8:door close
//6, C7:flash
//7, C6:door limit
GPIO_TypeDef * Door_GPIO_buf[8] = {GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC, GPIOC};
u16 Door_PIN_buf[8] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_13, GPIO_Pin_9, GPIO_Pin_8, GPIO_Pin_7, GPIO_Pin_6};
//GPIO_TypeDef * Door_GPIO_buf[4] = {GPIOB, GPIOC, GPIOC, GPIOC};
//u16 Door_PIN_buf[4] = {GPIO_Pin_15, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8};

bool is_door_closed(u8 num)
{
	if(GPIO_ReadInputDataBit(Door_GPIO_buf[num], Door_PIN_buf[num]) == Bit_SET)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

/*******************************************END OF FILE******************************************/

