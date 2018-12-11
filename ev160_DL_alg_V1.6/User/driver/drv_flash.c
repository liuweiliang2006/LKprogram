/******************************************************************************
* 文件名         : drv_flash.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，flash
*
*******************************************************************************/

#include "hw_lib.h"	

//STM_SECTOR_SIZE_BYTE * MAX_NUM
//SECTOR_ADR_START
//const u8 flash_data_init_buf[40960]__attribute__((at(0x08040000))) = {0};//MAX_NUM = 160
u32 dest_adr;
void flash_write_sector(u16 num, u16 * data_buf)
{
	u16 i;	
	if(num == 0) return;
	if(num > MAX_NUM) return;
	dest_adr = SECTOR_ADR(num);
	FLASH_Unlock();
	FLASH_ErasePage(dest_adr);
	for(i = 0;i < STM_SECTOR_SIZE_HALF_WORD;i++)
	{
		FLASH_ProgramHalfWord(dest_adr, *(data_buf + i));
		dest_adr+=2;
	}	
	FLASH_Lock();
}
void flash_read_sector(u16 num, u16 * data_buf)
{
	u16 i;	
	if(num == 0) return;
	if(num > MAX_NUM) return;
	dest_adr = SECTOR_ADR(num);
	for(i = 0;i < STM_SECTOR_SIZE_HALF_WORD;i++)
	{
		*(data_buf + i) = * (u16 *)dest_adr;
		dest_adr+=2;
	}	
}
#ifdef DEBUGTEST
u16 flash_test_data[STM_SECTOR_SIZE_HALF_WORD] = {0};
void test_flash_drv(void)
{
	u16 num, i;
	for(num = 1;num != MAX_NUM;num++)
	{
		for(i = 0;i < STM_SECTOR_SIZE_HALF_WORD;i++)
		{
			flash_test_data[i] = num;
		}
		flash_write_sector(num, (u16 *)flash_test_data);		
	}

}
#endif
/*******************************************END OF FILE******************************************/




