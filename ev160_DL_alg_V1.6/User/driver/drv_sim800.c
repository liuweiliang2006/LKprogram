/******************************************************************************
* 文件名         : drv_sim800.c
* 工程名         : *
* 作者           : pengwei
* 版本           : *
* 日期           : 2018.07
* 描述           : 驱动程序，sim800
*
*******************************************************************************/

#include "hw_lib.h"


char const str1[]="AT\r";                        //联机指令，返回OK
char const str2[]="AT+CMGF=1\r";                  //短信模式选择，1为文本模式，0为PDU
char const str3[]="AT+CSCS=\"UCS2\"\r";           //选择TE字符集
char const str4[]="AT+CSMP=17,0,2,25\r";          //设置短信息文本模式参数
char const str5[]="AT+CMGS=\"003800360031003800380031031003300320039003900330036\"\r"; //接收短信手机号
char const str6[]="60A8597DFF0C6B228FCE51494E345F7C5CB84FE1606F6280672FFF01\r";         //短信内容“你好，欢迎光临彼岸信息技术”
char const str7[]="AT+CSQ\r";                     //信号强度查询指令
char const str8[2]={0x1a, 0};                     //信号强度查询指令

static const char *SMS_NUM = "AT+CMGS=\"00310033003500350032003200350030003300300032\"";  // 接收方电话号码，请先用UNICODE工具转换好
//static const char *SMS_NUM = "AT+CMGS=\"00310033003500350032003200350030003300300032\"";  // 接收方电话号码，请先用UNICODE工具转换好
static const char *SMS_CONTENT10 = "949B65B979D16280FF0C8BA94E00520762E5670989E689C9FF01";  // 钛方科技，让一切拥有触觉！
//static const char *SMS_CONTENT10 = "949B65B96B228FCE60A8FF01";  // 钛方欢迎您！
//static const char *SMS_CONTENT11 = "00680065006C006C006F002C949B65B96B228FCE60A8FF01";  // hello,钛方欢迎您！
//static const char *SMS_CONTENT12 = "0031003200330034002C0061006200630064002C54C854C8FF01";  // 1234,abcd,哈哈！
//static const char *SMS_CONTENT1 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3524D8F6695E888AB657251FBFF01";  // 钛方提示：您的爱车，右前车门被敲击！
//static const char *SMS_CONTENT2 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3540E8F6695E888AB657251FBFF01";  // 钛方提示：您的爱车，右后车门被敲击！
//static const char *SMS_CONTENT3 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6524D8F6695E888AB657251FBFF01";  // 钛方提示：您的爱车，左前车门被敲击！
//static const char *SMS_CONTENT4 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6540E8F6695E888AB657251FBFF01";  // 钛方提示：您的爱车，左后车门被敲击！
//static const char *SMS_CONTENT5 = "949B65B963D0793AFF1A60A8768472318F66FF0C524D4FDD9669676088AB657251FBFF01";  // 钛方提示：您的爱车，前保险杠被敲击！
//static const char *SMS_CONTENT6 = "949B65B963D0793AFF1A60A8768472318F66FF0C540E4FDD9669676088AB657251FBFF01";  // 钛方提示：您的爱车，后保险杠被敲击！
static const char *SMS_CONTENT1 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3524D8F6695E853D1751F78B0649EFF01";  // 钛方提示：您的爱车，右前车门发生碰撞！
static const char *SMS_CONTENT2 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3540E8F6695E853D1751F78B0649EFF01";  // 钛方提示：您的爱车，右后车门发生碰撞！
static const char *SMS_CONTENT3 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6524D8F6695E853D1751F78B0649EFF01";  // 钛方提示：您的爱车，左前车门发生碰撞！
static const char *SMS_CONTENT4 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6540E8F6695E853D1751F78B0649EFF01";  // 钛方提示：您的爱车，左后车门发生碰撞！
static const char *SMS_CONTENT5 = "949B65B963D0793AFF1A60A8768472318F66FF0C524D4FDD9669676053D1751F78B0649EFF01";  // 钛方提示：您的爱车，前保险杠发生碰撞！
static const char *SMS_CONTENT6 = "949B65B963D0793AFF1A60A8768472318F66FF0C540E4FDD9669676053D1751F78B0649EFF01";  // 钛方提示：您的爱车，后保险杠发生碰撞！

#define SIM_REV_BUF_SIZE				256
char sim_buf[SIM_REV_BUF_SIZE];
u16 sim_buf_ptr = 0;
bool Admit_Sim_Rev = TRUE;


char pre_char = 0;
bool State_SimRev_Success = FALSE;
bool State_SimRev_0D0A1 = FALSE;
bool State_SimRev_Error = FALSE;
u8 sim_send_times = 0;

void sim_rev_irq_process(char rev_data)
{	
	if(Admit_Sim_Rev)
	{
		sim_buf[sim_buf_ptr++] = rev_data;
		if(sim_buf_ptr >= SIM_REV_BUF_SIZE)
		{
			sim_buf_ptr = 0;
		}
	}	
}
void sim_rev_clear_buf(void)
{
	Admit_Sim_Rev = FALSE;
	memset(sim_buf,0,SIM_REV_BUF_SIZE);
	sim_buf_ptr = 0;
	Admit_Sim_Rev = TRUE;
}
void sim_send_string(char * send_buf)
{
	u16 i = 0;	
	while(*(send_buf + i) != 0)
	{
		USART_SendData(USART3, (u16)(*(send_buf + i)));
		while (!(USART3->SR & USART_FLAG_TXE));		
		i++;
	}
}
/*******************************************************************************
* 函数名称: find_string
* 描    述: 判断缓存中是否含有指定的字符串
* 输入参数: 字符串 p
* 输    出: 无
* 返    回: unsigned char:1 找到指定字符，0 未找到指定字符
* 说    明: 调用extern char *strstr(char *str1, char *str2)该函数，
*******************************************************************************/

bool find_string(char* p)
{ 	
	if(strstr(sim_buf,p)!=NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
u8 sim_send_cmd(char * send_buf, char * comp_buf, u8 rep_times, u8 wait_time)
{
	u16 i = 0;
	sim_rev_clear_buf();
	
	while(i < rep_times)
	{
		sim_send_string(send_buf);
		sim_send_string("\r\n");
		delay_ms(wait_time);
		if(find_string(comp_buf))
		{
			return 1;
		}
		i++;
	}
	
	return 0;
}
void send_end(void)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); 
	USART_SendData(USART3 ,0X1A);   
}
void sim_test(void)
{
	u16 i = 1;
	if(is_door_closed(4) == TRUE)
	{
		delay_ms(2000); 
		
		// 判断核心板状态是否正常
		sim_send_cmd("AT", "OK", 2, 10);//1.握手成功,握手失败
		
		sim_send_cmd("AT+CPIN?", "+CPIN: READY", 2, 10);//2.读卡成功,读卡失败
		sim_send_cmd("AT+CREG?", "+CREG: 0,1", 2, 10);//3.注册成功,注册失败
		
		sim_send_cmd("AT+CMGF=1", "OK", 2, 10);//5.设置短信模式为文本模式,短信模式设置失败
		sim_send_cmd("AT+CSMP=17,167,1,8", "OK", 2, 10);//6.设置文本模式参数为中英文,参数设置失败
		sim_send_cmd("AT+CSCS=\"UCS2\"", "OK", 2, 10);//7.设置 UCS2 字符集,字符集设置失败
		// 设置接收方号码	，如果无误后发送短信内容,设置接收方手机号失败
		sim_send_cmd((char*)SMS_NUM, ">", 2, 20);//8.设置接收方号码
		sim_send_string((char*)SMS_CONTENT10);//9.写入短信内容
		delay_ms(100); 
		send_end();
		while(i)
		{
			if(find_string("+CMGS:"))
			{
				i = 0;
			}
		}
	}
//	sim_send_cmd("AT+CMGF=1", "OK", 2, 100);
//	sim_send_cmd("AT+CSMP=17,11,0,0", "OK", 2, 100);
//	sim_send_cmd("AT+CSCS= \"IRA\"", "OK", 2, 100);
//	sim_send_cmd("AT+CMGS=\"18611959960\"", ">", 2, 100);
////	delay_ms(100); 
//	sim_send_string("Hello,Your car is being hitted,Please be informed that");
////	delay_ms(100); 
//	send_end();

	
//	while(1);

}
void send_sms(u8 num)
{
	u16 i = 1;
	
	{
		sim_send_cmd("AT+CMGF=1", "OK", 2, 10);//5.设置短信模式为文本模式,短信模式设置失败
		sim_send_cmd("AT+CSMP=17,167,1,8", "OK", 2, 10);//6.设置文本模式参数为中英文,参数设置失败
		sim_send_cmd("AT+CSCS=\"UCS2\"", "OK", 2, 10);//7.设置 UCS2 字符集,字符集设置失败
		// 设置接收方号码	，如果无误后发送短信内容,设置接收方手机号失败
		sim_send_cmd((char*)SMS_NUM, ">", 2, 20);//8.设置接收方号码	
		
		switch(num)
		{
			case 1:
				sim_send_string((char*)SMS_CONTENT1);// 钛方提示：您的爱车，右前车门被敲击！
				break;
			case 2:
				sim_send_string((char*)SMS_CONTENT2);// 钛方提示：您的爱车，右后车门被敲击！
				break;
			case 3:
				sim_send_string((char*)SMS_CONTENT3);// 钛方提示：您的爱车，左前车门被敲击！
				break;
			case 4:
				sim_send_string((char*)SMS_CONTENT4);// 钛方提示：您的爱车，左后车门被敲击！
				break;
			case 5:
				sim_send_string((char*)SMS_CONTENT5);// 钛方提示：您的爱车，前保险杠被敲击！
				break;
			case 6:
				sim_send_string((char*)SMS_CONTENT6);// 钛方提示：您的爱车，后保险杠被敲击！
				break;
			default:
				sim_send_string((char*)SMS_CONTENT10);// 钛方欢迎您！
				break;
		}
		delay_ms(100); 
		send_end();
		while(i)
		{
			if(find_string("+CMGS:"))
			{
				i = 0;
			}
		}
	}	
}

/*******************************************END OF FILE******************************************/
