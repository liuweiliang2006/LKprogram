/******************************************************************************
* �ļ���         : drv_dog.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : 2018.07
* ����           : �������򣬿��Ź�����
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
	IWDG_SetPrescaler(IWDG_PRE); //���� IWDG Ԥ��Ƶֵ
	IWDG_SetReload(IWDG_RLD); //���� IWDG ��װ��ֵ

}
void dog_en(void)
{
	IWDG_Enable(); //ʹ�� IWDG
}
void dog_eat(void)
{
	IWDG_ReloadCounter(); //���� IWDG ��װ�ؼĴ�����ֵ��װ�� IWDG ������
}
#endif

/*******************************************END OF FILE******************************************/
