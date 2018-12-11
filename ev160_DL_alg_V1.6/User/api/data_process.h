

#include "stm32f10x.h"

#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

#define DOOR_RIGHT_FRONT_VALUE_N						10000
#define DOOR_RIGHT_REAR_VALUE_N							10000
#define DOOR_LEFT_FRONT_VALUE_N							10000
#define DOOR_LEFT_REAR_VALUE_N							10000
#define BUMPER_FRONT_VALUE_N								10000
#define BUMPER_REAR_VALUE_N									10000

#define DOOR_RIGHT_FRONT_VALUE_H						30000
#define DOOR_RIGHT_REAR_VALUE_H							30000
#define DOOR_LEFT_FRONT_VALUE_H							30000
#define DOOR_LEFT_REAR_VALUE_H							30000
#define BUMPER_FRONT_VALUE_H								30000
#define BUMPER_REAR_VALUE_H									30000

#define TIMER_GREAT													0xFAF5FAF5
#define TIMER_POS_1													0//计算离上次采样的时间

#define WAVE_DT_NUM									0//数据处理判断使用的延时定时器编号

#define WAVE_TIME_INTER							80//ms
#define WAVE_TIME								100//ms
#define WAVE_NUM								3//
#define ENERGY_MAX_RATIO						1//energy_max1与energy_max2的比值
#define AFTERWIND_RATIO							2
#define LOWENERGY								300
#define WAITE_1024_T							60									
//#define ENERGY_MAX_VALUE						60000
#define ENERGY_MAX_VALUE1_L						8000//70000
#define ENERGY_MAX_VALUE2_L						8000//120000
#define ENERGY_MAX_VALUE3_L						8000//120000
#define ENERGY_MAX_VALUE4_L						8000//80000
#define ENERGY_MAX_VALUE5_L						30000//40000//15000
#define ENERGY_MAX_VALUE6_L						30000//30000//15000
#define ENERGY_MAX_VALUE7_L						30000//20000//15000
#define ENERGY_MAX_VALUE8_L						30000//30000//15000

#define ENERGY_MAX_VALUE1						40000//70000//10000
#define ENERGY_MAX_VALUE2						80000//120000//20000
#define ENERGY_MAX_VALUE3						70000//120000//15000
#define ENERGY_MAX_VALUE4						60000//80000///15000 
#define ENERGY_MAX_VALUE5						25000//25000//40000
#define ENERGY_MAX_VALUE6						25000//30000//30000
#define ENERGY_MAX_VALUE7						25000//30000//20000
#define ENERGY_MAX_VALUE8						25000//40000//30000

typedef enum {NONE = 0, CRASH = 1,SCRATCH = 2} Report_Kind;//无触发，碰撞，剐蹭
typedef struct DATA_JUGE
{
	u8 max1_i;
	u8 max2_i;
	u32 energy_max1; 
	u32 energy_max2;
	u32 energy_pre;
	u32 wave_time;
	u8 wave_num;	
	Report_Kind report_result;
	u8 report_pos;
}DATA_JUGE_Type;	


DATA_JUGE_Type *  data_process_lowdetect(u16 AD_store_final[][512], u8 channel_num);

#endif




