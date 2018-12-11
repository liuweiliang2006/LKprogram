/******************************************************************************
* 文件名         : drv_dog.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，看门狗驱动
*
*******************************************************************************/

#include "hw_lib.h"

#define IWDG_PRE			IWDG_Prescaler_256			//40khz, 64->1.6ms, 128->3.2ms, 256->6.4ms
//#define IWDG_RLD			250											//6.4ms, 1.6s
#define IWDG_RLD			150											//6.4ms, 0.96s

#ifdef IWDG_FUN
void dog_init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_PRE); //设置 IWDG 预分频值
	IWDG_SetReload(IWDG_RLD); //设置 IWDG 重装载值

}
void dog_en(void)
{
	IWDG_Enable(); //使能 IWDG
}
void dog_eat(void)
{
	IWDG_ReloadCounter(); //按照 IWDG 重装载寄存器的值重装载 IWDG 计数器
}
#endif

/*******************************************END OF FILE******************************************/
