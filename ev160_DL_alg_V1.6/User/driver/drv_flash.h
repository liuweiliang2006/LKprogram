#include "stm32f10x.h"

#ifndef DRV_FLASH_H
#define DRV_FLASH_H

#define STM_FLASH_ADR_START					0x08000000
#define STM_FLASH_ADR_SIZE					0x80000
#define STM_FLASH_ADR_END						0x08080000


#define STM_SECTOR_SIZE_BYTE 				2048
#define STM_SECTOR_SIZE_HALF_WORD		1024
#define STM_SECTOR_SIZE_WORD		 		512

//#define MAX_NUM											10//10->9->3
//#define MAX_NUM											31//31->30->10
#define MAX_NUM											151//151->150->50
//#define MAX_NUM											181//181->180->60
#define SECTOR_ADR(num)							(STM_FLASH_ADR_END - (num) * STM_SECTOR_SIZE_BYTE)
#define SECTOR_ADR_START						(STM_FLASH_ADR_END - MAX_NUM * STM_SECTOR_SIZE_BYTE)

void flash_read_sector(u16 num, u16 * data_buf);
void flash_write_sector(u16 num, u16 * data_buf);

#ifdef DEBUGTEST
void test_flash_drv(void);
#endif

#endif


