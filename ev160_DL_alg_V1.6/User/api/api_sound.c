/******************************************************************************
* 文件名         : api_sound.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : *
* 描述           : 接口程序，语音播报接口程序
*
*******************************************************************************/

#include "hw_lib.h"

#define SOUND_ON		

const u8 s01_inquire_play[4] = {0xAA,0x01,0x00,0xAB};//查询播放状态,返回5个字节,AA 01 01 播放状态 SM
const u8 s02_data_spk[4] = {0xAA,0x02,0x00,0xAC};//语音播放
const u8 s05_next_spk[4] = {0xAA,0x05,0x00,0xAF};//下一曲
const u8 s06_previous_spk[4] = {0xAA,0x06,0x00,0xB0};//上一曲
const u8 s07_qumu1[6] = {0xAA,0x07,0x02,0x00,0x01,0xB4}; 
const u8 s07_qumu2[6] = {0xAA,0x07,0x02,0x00,0x02,0xB5}; 
const u8 s07_qumu3[6] = {0xAA,0x07,0x02,0x00,0x03,0xB6}; 
const u8 s07_qumu4[6] = {0xAA,0x07,0x02,0x00,0x04,0xB7}; 
const u8 s07_qumu5[6] = {0xAA,0x07,0x02,0x00,0x05,0xB8}; 
const u8 s07_qumu6[6] = {0xAA,0x07,0x02,0x00,0x06,0xB9}; 
const u8 s07_qumu7[6] = {0xAA,0x07,0x02,0x00,0x07,0xBA}; 
const u8 s07_qumu8[6] = {0xAA,0x07,0x02,0x00,0x08,0xBB}; 
const u8 s07_data_play[4] = {0xAA,0x07,0x02,0x00};  //实际语音播放选择
const u8 s0b_data_flash[5] = {0xAA,0x0B,0x01,0x02,0xB8};//切换到FLASH
const u8 s13_data_vol[5] = {0xAA,0x13,0x01,0x1E,0xDC};//音量设置
const u8 s14_vol_up[4] = {0xAA,0x14,0x00,0xBE};//音量加
const u8 s15_vol_down[4] = {0xAA,0x15,0x00,0xBF};//音量减
u8 sound_back_buf[16] = {0};
void play_sound(u8 num)
{
#ifdef SOUND_ON
	u8 temp_buf[6];
	u8 sum = 0;
	u8 i = 0;
    
    if(is_door_closed(4) == TRUE)
    {
        num += 16;
    }
	while(1)
	{
		sound_senddata((u8*)s01_inquire_play, sizeof(s01_inquire_play));//查询播放状态,返回5个字节,AA 01 01 播放状态 SM
		if(sound_getdata(sound_back_buf) != 0) if(sound_back_buf[3] != 1) break;
	}
	
	temp_buf[i++] = 0xAA;sum += 0xAA;
	temp_buf[i++] = 0x07;sum += 0x07;
	temp_buf[i++] = 0x02;sum += 0x02;
	temp_buf[i++] = 0x00;sum += 0x00;
	temp_buf[i++] = num;sum += num;
	temp_buf[i++] = sum;
	
	sound_senddata(temp_buf, i);
#endif
}
void sound_vol_set(u8 num)
{
	u8 temp_buf[5];
	u8 sum = 0;
	u8 i = 0;
	
	temp_buf[i++] = 0xAA;sum += 0xAA;
	temp_buf[i++] = 0x13;sum += 0x13;
	temp_buf[i++] = 0x01;sum += 0x01;
	temp_buf[i++] = num; sum += num;
	temp_buf[i++] = sum;
	
	sound_senddata(temp_buf, i);
}
void sound_vol_up(void)
{
	sound_senddata((u8*)s14_vol_up, sizeof(s14_vol_up));
}
void sound_vol_down(void)
{
	sound_senddata((u8*)s15_vol_down, sizeof(s15_vol_down));
}

#ifdef DEBUGTEST
void sound_test(void)
{	
	u16 i = 1;
	while(1)
	{
		play_sound(i++);
		if(i == 17)
		{
			i = 1;
		}
		
//		led_twinkle(k++, 2, 500);if(k == 5) k = 0;
//		sound_vol_set(30);
	}
}
#endif

/*******************************************END OF FILE******************************************/



