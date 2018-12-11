/******************************************************************************
* 文件名         : drv_led.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，LED
*
*******************************************************************************/

#include "hw_lib.h"	

//0:A3:LED0, 1:B3:LED1, 2:B4:LED2, 3:B5:LED3, 4:C12:IO OUT, 5:B8:12V ctl
GPIO_TypeDef * LED_GPIO_buf[6] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOC, GPIOB};
u16 LED_PIN_buf[6] = {GPIO_Pin_15, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_12, GPIO_Pin_8};

void led_on(u8 num)
{
	GPIO_SetBits(LED_GPIO_buf[num], LED_PIN_buf[num]);	
}
void led_off(u8 num)
{
	GPIO_ResetBits(LED_GPIO_buf[num], LED_PIN_buf[num]);	
}


/*******************************************END OF FILE******************************************/




