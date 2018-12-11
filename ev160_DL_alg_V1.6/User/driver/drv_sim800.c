/******************************************************************************
* �ļ���         : drv_sim800.c
* ������         : *
* ����           : pengwei
* �汾           : *
* ����           : 2018.07
* ����           : ��������sim800
*
*******************************************************************************/

#include "hw_lib.h"


char const str1[]="AT\r";                        //����ָ�����OK
char const str2[]="AT+CMGF=1\r";                  //����ģʽѡ��1Ϊ�ı�ģʽ��0ΪPDU
char const str3[]="AT+CSCS=\"UCS2\"\r";           //ѡ��TE�ַ���
char const str4[]="AT+CSMP=17,0,2,25\r";          //���ö���Ϣ�ı�ģʽ����
char const str5[]="AT+CMGS=\"003800360031003800380031031003300320039003900330036\"\r"; //���ն����ֻ���
char const str6[]="60A8597DFF0C6B228FCE51494E345F7C5CB84FE1606F6280672FFF01\r";         //�������ݡ���ã���ӭ���ٱ˰���Ϣ������
char const str7[]="AT+CSQ\r";                     //�ź�ǿ�Ȳ�ѯָ��
char const str8[2]={0x1a, 0};                     //�ź�ǿ�Ȳ�ѯָ��

static const char *SMS_NUM = "AT+CMGS=\"00310033003500350032003200350030003300300032\"";  // ���շ��绰���룬������UNICODE����ת����
//static const char *SMS_NUM = "AT+CMGS=\"00310033003500350032003200350030003300300032\"";  // ���շ��绰���룬������UNICODE����ת����
static const char *SMS_CONTENT10 = "949B65B979D16280FF0C8BA94E00520762E5670989E689C9FF01";  // �ѷ��Ƽ�����һ��ӵ�д�����
//static const char *SMS_CONTENT10 = "949B65B96B228FCE60A8FF01";  // �ѷ���ӭ����
//static const char *SMS_CONTENT11 = "00680065006C006C006F002C949B65B96B228FCE60A8FF01";  // hello,�ѷ���ӭ����
//static const char *SMS_CONTENT12 = "0031003200330034002C0061006200630064002C54C854C8FF01";  // 1234,abcd,������
//static const char *SMS_CONTENT1 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3524D8F6695E888AB657251FBFF01";  // �ѷ���ʾ�����İ�������ǰ���ű��û���
//static const char *SMS_CONTENT2 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3540E8F6695E888AB657251FBFF01";  // �ѷ���ʾ�����İ������Һ��ű��û���
//static const char *SMS_CONTENT3 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6524D8F6695E888AB657251FBFF01";  // �ѷ���ʾ�����İ�������ǰ���ű��û���
//static const char *SMS_CONTENT4 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6540E8F6695E888AB657251FBFF01";  // �ѷ���ʾ�����İ���������ű��û���
//static const char *SMS_CONTENT5 = "949B65B963D0793AFF1A60A8768472318F66FF0C524D4FDD9669676088AB657251FBFF01";  // �ѷ���ʾ�����İ�����ǰ���ոܱ��û���
//static const char *SMS_CONTENT6 = "949B65B963D0793AFF1A60A8768472318F66FF0C540E4FDD9669676088AB657251FBFF01";  // �ѷ���ʾ�����İ��������ոܱ��û���
static const char *SMS_CONTENT1 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3524D8F6695E853D1751F78B0649EFF01";  // �ѷ���ʾ�����İ�������ǰ���ŷ�����ײ��
static const char *SMS_CONTENT2 = "949B65B963D0793AFF1A60A8768472318F66FF0C53F3540E8F6695E853D1751F78B0649EFF01";  // �ѷ���ʾ�����İ������Һ��ŷ�����ײ��
static const char *SMS_CONTENT3 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6524D8F6695E853D1751F78B0649EFF01";  // �ѷ���ʾ�����İ�������ǰ���ŷ�����ײ��
static const char *SMS_CONTENT4 = "949B65B963D0793AFF1A60A8768472318F66FF0C5DE6540E8F6695E853D1751F78B0649EFF01";  // �ѷ���ʾ�����İ���������ŷ�����ײ��
static const char *SMS_CONTENT5 = "949B65B963D0793AFF1A60A8768472318F66FF0C524D4FDD9669676053D1751F78B0649EFF01";  // �ѷ���ʾ�����İ�����ǰ���ոܷ�����ײ��
static const char *SMS_CONTENT6 = "949B65B963D0793AFF1A60A8768472318F66FF0C540E4FDD9669676053D1751F78B0649EFF01";  // �ѷ���ʾ�����İ��������ոܷ�����ײ��

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
* ��������: find_string
* ��    ��: �жϻ������Ƿ���ָ�����ַ���
* �������: �ַ��� p
* ��    ��: ��
* ��    ��: unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ�
* ˵    ��: ����extern char *strstr(char *str1, char *str2)�ú�����
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
		
		// �жϺ��İ�״̬�Ƿ�����
		sim_send_cmd("AT", "OK", 2, 10);//1.���ֳɹ�,����ʧ��
		
		sim_send_cmd("AT+CPIN?", "+CPIN: READY", 2, 10);//2.�����ɹ�,����ʧ��
		sim_send_cmd("AT+CREG?", "+CREG: 0,1", 2, 10);//3.ע��ɹ�,ע��ʧ��
		
		sim_send_cmd("AT+CMGF=1", "OK", 2, 10);//5.���ö���ģʽΪ�ı�ģʽ,����ģʽ����ʧ��
		sim_send_cmd("AT+CSMP=17,167,1,8", "OK", 2, 10);//6.�����ı�ģʽ����Ϊ��Ӣ��,��������ʧ��
		sim_send_cmd("AT+CSCS=\"UCS2\"", "OK", 2, 10);//7.���� UCS2 �ַ���,�ַ�������ʧ��
		// ���ý��շ�����	�����������Ͷ�������,���ý��շ��ֻ���ʧ��
		sim_send_cmd((char*)SMS_NUM, ">", 2, 20);//8.���ý��շ�����
		sim_send_string((char*)SMS_CONTENT10);//9.д���������
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
		sim_send_cmd("AT+CMGF=1", "OK", 2, 10);//5.���ö���ģʽΪ�ı�ģʽ,����ģʽ����ʧ��
		sim_send_cmd("AT+CSMP=17,167,1,8", "OK", 2, 10);//6.�����ı�ģʽ����Ϊ��Ӣ��,��������ʧ��
		sim_send_cmd("AT+CSCS=\"UCS2\"", "OK", 2, 10);//7.���� UCS2 �ַ���,�ַ�������ʧ��
		// ���ý��շ�����	�����������Ͷ�������,���ý��շ��ֻ���ʧ��
		sim_send_cmd((char*)SMS_NUM, ">", 2, 20);//8.���ý��շ�����	
		
		switch(num)
		{
			case 1:
				sim_send_string((char*)SMS_CONTENT1);// �ѷ���ʾ�����İ�������ǰ���ű��û���
				break;
			case 2:
				sim_send_string((char*)SMS_CONTENT2);// �ѷ���ʾ�����İ������Һ��ű��û���
				break;
			case 3:
				sim_send_string((char*)SMS_CONTENT3);// �ѷ���ʾ�����İ�������ǰ���ű��û���
				break;
			case 4:
				sim_send_string((char*)SMS_CONTENT4);// �ѷ���ʾ�����İ���������ű��û���
				break;
			case 5:
				sim_send_string((char*)SMS_CONTENT5);// �ѷ���ʾ�����İ�����ǰ���ոܱ��û���
				break;
			case 6:
				sim_send_string((char*)SMS_CONTENT6);// �ѷ���ʾ�����İ��������ոܱ��û���
				break;
			default:
				sim_send_string((char*)SMS_CONTENT10);// �ѷ���ӭ����
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
